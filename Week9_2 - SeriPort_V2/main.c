#include <yedlib.h>

static uint32_t hours = 0;
static uint32_t minutes = 0;
static uint32_t seconds = 0;

// UART interrupt handler
void UARTIntHandler(void)
{
    uint32_t status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, status);

    static char messageBuffer[64]; // Buffer to store incoming messages
    static int bufferIndex = 0;    // Current index in the buffer
    UARTCharPut(UART0_BASE, '*');
    LCD_GoTo(0, 0);
    while (UARTCharsAvail(UART0_BASE))
    {
        char receivedChar = UARTCharGet(UART0_BASE);

        if (receivedChar >= 32 && receivedChar <= 126)
        {
            if (bufferIndex < sizeof(messageBuffer) - 1) // Check buffer limit
            {
                UARTCharPut(UART0_BASE, receivedChar);
                messageBuffer[bufferIndex++] = receivedChar; // Add character to the buffer
            }
        }
        else if (receivedChar == '\n') // End of message
        {
            messageBuffer[bufferIndex] = '\0'; // Null-terminate the buffer
            LCD_Print(messageBuffer);         // Print the entire message on the LCD
            memset(messageBuffer, 0, sizeof(messageBuffer) - 1);
            bufferIndex = 0;                  // Reset the buffer index
        }
    }
    UARTCharPut(UART0_BASE, '\n');
}

// Timer interrupt handler
void TimerIntHandler(void)
{
    char timeBuffer[16];

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    seconds++;
    if (seconds >= 60)
    {
        seconds = 0;
        minutes++;
    }
    if (minutes >= 60)
    {
        minutes = 0;
        hours++;
    }
    if (hours >= 24)
    {
        hours = 0;
    }

    snprintf(timeBuffer, sizeof(timeBuffer), "%02u:%02u:%02u", hours, minutes, seconds);

    LCD_GoTo(1, 8);
    LCD_Print(timeBuffer);
    int i;
    UARTCharPut(UART0_BASE, '#');
    for (i = 0; timeBuffer[i] != '\0'; i++)
    {
        UARTCharPut(UART0_BASE, timeBuffer[i]);
    }
    UARTCharPut(UART0_BASE, '\n');
}

// Read the initial time from UART
void ReadInitialTime(void)
{
    char receivedChar;
    char timeBuffer[16];
    int index = 0;

    while (1)
    {
        if (UARTCharsAvail(UART0_BASE))
        {
            receivedChar = UARTCharGet(UART0_BASE);

            if (receivedChar == '\n')
            {
                timeBuffer[index] = '\0';
                sscanf(timeBuffer, "%02u:%02u:%02u", &hours, &minutes, &seconds);
                break;
            }
            else if (index < sizeof(timeBuffer) - 1)
            {
                timeBuffer[index++] = receivedChar;
            }
        }
    }
}

void DisplayADCValueOnLCD(uint32_t adcValue);

uint32_t value[1]; // Stores the value obtained from the ADC

// ADC interrupt service routine
void ADC0Handler(void) {
    // Clear ADC interrupt flag
    ADCIntClear(ADC0_BASE, 3);

    // Get ADC result
    ADCSequenceDataGet(ADC0_BASE, 3, value);

    // Display ADC data on the LCD
    DisplayADCValueOnLCD(value[0]);
}

// Simplified formatting function instead of snprintf
void DisplayADCValueOnLCD(uint32_t adcValue) {
    char buffer[6]; // Buffer to display on the LCD
    int intVoltage = (adcValue * 3300) / 4096; // Calculate voltage in mV
    memset(buffer, 0, 5);

    buffer[0] = (intVoltage / 1000) + '0'; // Integer part of the voltage
    buffer[1] = '.';
    buffer[2] = ((intVoltage / 100) % 10) + '0'; // First decimal place
    buffer[3] = ((intVoltage / 10) % 10) + '0';  // Second decimal place
    buffer[4] = 'V';
    buffer[5] = 0;
    // Print to LCD
    LCD_GoTo(1, 0);
    LCD_Print(buffer);
    int i;
    UARTCharPut(UART0_BASE, '+');
    for (i = 0; buffer[i] != '\0'; i++)
    {
        UARTCharPut(UART0_BASE, buffer[i]);
    }
    UARTCharPut(UART0_BASE, '\n');
}

void ADC0Init(void)
{
    // Enable GPIOF and ADC0 peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    TimerControlTrigger(TIMER0_BASE, TIMER_A, true); // Enable Timer for ADC triggering

    // Configure ADC0 Sequence 3 (Timer-triggered)
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);

    // Configure steps for Sequence 3 (1 sample via CH0)
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

    // Enable Sequence 3
    ADCSequenceEnable(ADC0_BASE, 3);

    // Enable ADC interrupts
    IntMasterEnable();
    ADCIntRegister(ADC0_BASE, 3, ADC0Handler);
    ADCIntEnable(ADC0_BASE, 3);
}

static void init()
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    LCD_Init();
    LCD_Cmd(GPIO_PIN_0);
    LCD_Cmd(GPIO_PIN_7);
    delay_ms(100);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTEnable(UART0_BASE);

    ReadInitialTime();

    UARTFIFOEnable(UART0_BASE);

    // Enable UART interrupts
    UARTIntRegister(UART0_BASE, UARTIntHandler); // Register interrupt function
    IntEnable(INT_UART0);                        // Enable UART0 interrupts
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PE
