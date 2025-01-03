#include "yedlib.h"

int main(void)
{
    SetInitSettings();                        // Initial settings

    while(1)
    {
        // Continuous wait in the main loop.
    }
}

void SetInitSettings()
{
    // Set system clock to 40 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Enable GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);   // Set pins 1, 2, and 3 as outputs

    // Enable Timer0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC);   // Configure Timer0 A and B in separate periodic modes

    // Set TimerA load value for 0.5-second interrupts
    TimerLoadSet(TIMER0_BASE, TIMER_A, 50000);  // Set to 50000 * 400 = 20 million cycles for 0.5-second interval at 40 MHz

    // Set TimerB load value for 1-second interrupts
    TimerLoadSet(TIMER0_BASE, TIMER_B, 50000);  // Set to 50000 * 800 = 40 million cycles for 1-second interval at 40 MHz

    // Interrupt settings - enable global and local interrupts
    IntMasterEnable();                              // Enable global interrupts
    IntEnable(INT_TIMER0A);                         // Enable interrupt for Timer0A
    IntEnable(INT_TIMER0B);                         // Enable interrupt for Timer0B

    // Enable interrupts for TimerA and TimerB
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Enable interrupt for TimerA timeout
    TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT); // Enable interrupt for TimerB timeout

    // Register interrupt handlers for TimerA and TimerB
    TimerIntRegister(TIMER0_BASE, TIMER_A, timerAInterruptHandler);
    TimerIntRegister(TIMER0_BASE, TIMER_B, timerBInterruptHandler);

    // Start TimerA and TimerB
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER0_BASE, TIMER_B);
}

void timerAInterruptHandler()
{
    // Clear TimerA interrupt flag
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    timerACounter++;

    if(timerACounter == TIMER_A_OVERFLOW_LIMIT)
    {
        // Toggle LED on pin 1 every 0.5 seconds
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
        timerACounter = 0;
    }
}

void timerBInterruptHandler()
{
    // Clear TimerB interrupt flag
    TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
    timerBCounter++;

    if(timerBCounter == TIMER_B_OVERFLOW_LIMIT)
    {
        // Toggle LED on pin 2 every 1 second
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2));
        timerBCounter = 0;
    }
}
