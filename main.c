#include "yedlib.h"

void pfkesme(void)
{
    uint32_t status = GPIOIntStatus(GPIO_PORTF_BASE, true);
    GPIOIntClear(GPIO_PORTF_BASE, status); // Clear the interrupt flag

    if (status & GPIO_PIN_4) // If PF4 interrupt occurred - sw1
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // Turn on PF1 LED (red LED)
    else if (status & GPIO_PIN_0) // If PF0 interrupt occurred - sw2
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Turn on PF2 LED (blue LED)
}

void timerkesme(void)
{
    // Clear TimerA interrupt flag
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
}

void SetInitSettings()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);   // Set pins 1, 2, and 3 as outputs
    HWREG(GPIO_PORTF_BASE + (GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_5) << 2) = (GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_5); // Flash the leds

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure edge interrupts for PF0 and PF4, falling edge (when button is pressed)
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOIntRegister(GPIO_PORTF_BASE, pfkesme);
    IntEnable(INT_GPIOF);
    IntMasterEnable();

    // Enable Timer0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);   // Configure Timer0 A separate periodic modes

    // Set TimerA load value for 0.5-second interrupts
    TimerLoadSet(TIMER0_BASE, TIMER_A, 4000000);

    // Interrupt settings - enable global and local interrupts
    IntMasterEnable();                              // Enable global interrupts
    IntEnable(INT_TIMER0A);                         // Enable interrupt for Timer0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Enable interrupt for TimerA timeout

    TimerIntRegister(TIMER0_BASE, TIMER_A, timerkesme);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

int main(void)
{
    // Set system clock to 40 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SetInitSettings();                        // Initial settings

    while(1)
    {
        // Continuous wait in the main loop.
    }
}
