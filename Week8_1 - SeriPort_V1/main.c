#include <yedlib.h>

static uint32_t hours = 0;
static uint32_t minutes = 0;
static uint32_t seconds = 0;

void UARTIntHandler(void)
{
    uint32_t status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, status);

    static char messageBuffer[64]; // Gelen mesajlarý saklamak için buffer
    static int bufferIndex = 0;   // Buffer içindeki mevcut indeks
    UARTCharPut(UART0_BASE, '*');
    LCD_GoTo(0, 0);
    while (UARTCharsAvail(UART0_BASE))
    {
        char receivedChar = UARTCharGet(UART0_BASE);

        if (receivedChar >= 32 && receivedChar <= 126)
        {
            if (bufferIndex < sizeof(messageBuffer) - 1) // Buffer sýnýrýný kontrol et
            {
                UARTCharPut(UART0_BASE, receivedChar);
                messageBuffer[bufferIndex++] = receivedChar; // Karakteri buffer'a ekle
            }
        }
        else if (receivedChar == '\n') // Mesaj sonu geldiðinde
        {
            messageBuffer[bufferIndex] = '\0'; // Buffer'ý null-terminate yap
            LCD_Print(messageBuffer);  // Buffer'daki tüm metni yazdýr
            memset(messageBuffer, 0, sizeof(messageBuffer) - 1);
            bufferIndex = 0;           // Buffer indeksini sýfýrla
        }
    }
    UARTCharPut(UART0_BASE, '\n');
}

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

    // UART kesmeleri etkinleþtir
    UARTIntRegister(UART0_BASE, UARTIntHandler); // Kesme fonksiyonunu kaydet
    IntEnable(INT_UART0);                       // UART0 kesmelerini etkinleþtir
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());

    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, TimerIntHandler);

    TimerEnable(TIMER0_BASE, TIMER_A);
}

int main(void)
{
    init();
    while (1)
    {

    }
}
