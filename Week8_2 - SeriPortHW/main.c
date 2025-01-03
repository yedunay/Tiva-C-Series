#include <yedlib.h>

void TimerIntHandler(void)
{
    char receivedChar;
    static char timeBuffer[16]; // HH:mm:ss + null
    static int index = 0;

    while (UARTCharsAvail(UART0_BASE)) // Check if there are characters to read
    {
        receivedChar = UARTCharGet(UART0_BASE);

        if (receivedChar == '\n') // End of time data
        {
            timeBuffer[index] = '\0'; // Null terminate the string
            index = 0;

            LCD_GoTo(1, 0);
            LCD_Print(timeBuffer);

            // Geri gönder
            int i;
            for (i = 0; timeBuffer[i] != '\0'; i++)
            {
                UARTCharPut(UART0_BASE, timeBuffer[i]);
            }
            UARTCharPut(UART0_BASE, '\n'); // Terminate with newline
        }
        else if (index < sizeof(timeBuffer) - 1)
        {
            timeBuffer[index++] = receivedChar;
        }
    }
}


static void init()
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    LCD_Init();
    LCD_Cmd(GPIO_PIN_0); // Clear the screen
    LCD_Cmd(GPIO_PIN_7); // Set the cursor home
    delay_ms(100);
    LCD_GoTo(0, 0);
    LCD_Print("Time :");

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, // Baud rate: 9600
                        (UART_CONFIG_WLEN_8 |               // 8-bit veri uzunluðu
                         UART_CONFIG_STOP_ONE |             // 1 stop biti
                         UART_CONFIG_PAR_NONE));
    UARTFIFOEnable(UART0_BASE);
    UARTEnable(UART0_BASE);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);             // Enable Timer 0 peripheral
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);        // Configure Timer A as periodic (down counter)
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());     // Load Timer A with 40,000,000 for 1-second interval

    // Global interrupt settings
    IntMasterEnable();                                        // Enable global interrupts
    IntEnable(INT_TIMER0A);                                   // Enable Timer 0A interrupt

    // Local interrupt settings
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);          // Enable timeout interrupt for Timer A
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);           // Clear interrupt flag
    TimerIntRegister(TIMER0_BASE, TIMER_A, TimerIntHandler);  	  // Register the interrupt handler function

    TimerEnable(TIMER0_BASE, TIMER_A);                        // Start Timer A
}


int main(void)
{
	init();
    while(1)
    {
    }
}
