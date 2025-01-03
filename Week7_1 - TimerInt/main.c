#include <yedlib.h>

int main(void)
{
    SetInitSettings();                     // Initial settings

    while(1)
    {
        // Main loop does nothing, waits for interrupts
    }
}

void SetInitSettings()
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);   // Set clock to 40MHz

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);              // Enable GPIO Port F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);   // Set pins 1, 2, and 3 as outputs

    // Timer settings
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);             // Enable Timer 0 peripheral
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);        // Configure Timer A as periodic (down counter)
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());     // Load Timer A with 40,000,000 for 1-second interval

    // Global interrupt settings
    IntMasterEnable();                                        // Enable global interrupts
    IntEnable(INT_TIMER0A);                                   // Enable Timer 0A interrupt

    // Local interrupt settings
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);          // Enable timeout interrupt for Timer A
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);           // Clear interrupt flag
    TimerIntRegister(TIMER0_BASE, TIMER_A, MyInterruptFunc);  // Register the interrupt handler function

    TimerEnable(TIMER0_BASE, TIMER_A);                        // Start Timer A
}

void MyInterruptFunc()
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);           // Clear interrupt flag

    uint32_t deger = GPIOPinRead(GPIO_PORTF_BASE, 2);         // Read the state of the LED on pin 2
    GPIOPinWrite(GPIO_PORTF_BASE, 2, ~deger);                 // Toggle LED state by writing the inverse
}
