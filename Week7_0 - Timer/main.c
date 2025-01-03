#include <yedlib.h>

int main(void)
{
    SetInitSettings();                 // Perform initial settings

    while(1)
    {
        // Check if Timer0 has completed counting (timeout)
        uint32_t tdeger = TimerValueGet(TIMER0_BASE, TIMER_A);

        if(tdeger < 1000)
        {
            // Read the LED state and toggle it
            uint8_t ledState = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1);
            if(ledState)
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);            // Turn off LED
            else
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);   // Turn on LED
            // Timer must be reloaded and started
            TimerEnable(TIMER0_BASE, TIMER_A);                           // Restart the Timer
        }
    }
}

void SetInitSettings()
{
    // Configure clock to 40 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Enable GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Configure PF1 (red LED) as output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    // Turn off the LED initially
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

    // Enable Timer 0 peripheral unit
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    // Configure Timer0, Timer A as one-shot mode
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);

    // Load Timer A with 1 second (40 million cycles)
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());

    // Start the Timer
    TimerEnable(TIMER0_BASE, TIMER_A);                                    // setb tr0 in asm
}
