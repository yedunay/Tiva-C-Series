#include <yedlib.h>

volatile int sec = 0;
volatile int min = 0;
volatile int hour = 0;

volatile int now_h = 12, now_m = 37, now_s = 10;

void StopWatch(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);           // Clear interrupt flag

	if(++sec == 60)
	{
		sec = 0;
		if(++min == 60)
		{
			min = 0;
			if(++hour == 24)
				hour = 0;
		}
	}
	char timeStr[9];
	snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hour, min, sec);
	LCD_GoTo(1, 8);
	LCD_Print(timeStr);

	if(++now_s == 60)
	{
		now_s = 0;
		if(++now_m == 60)
		{
			now_m = 0;
			if(++now_h == 24)
				now_h = 0;
		}
	}
	snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", now_h, now_m, now_s);
	LCD_GoTo(0, 8);
	LCD_Print(timeStr);
}

static void init()
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    LCD_Init();
    LCD_Cmd(GPIO_PIN_0); // Clear the screen
    LCD_Cmd(GPIO_PIN_7); // Set the cursor home
    delay_ms(100);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);             // Enable Timer 0 peripheral
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);        // Configure Timer A as periodic (down counter)
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());     // Load Timer A with 40,000,000 for 1-second interval

    // Global interrupt settings
    IntMasterEnable();                                        // Enable global interrupts
    IntEnable(INT_TIMER0A);                                   // Enable Timer 0A interrupt

    // Local interrupt settings
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);          // Enable timeout interrupt for Timer A
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);           // Clear interrupt flag
    TimerIntRegister(TIMER0_BASE, TIMER_A, StopWatch);  	  // Register the interrupt handler function

    TimerEnable(TIMER0_BASE, TIMER_A);                        // Start Timer A
}

int main(void)
{
	init();
	LCD_GoTo(0, 0);
	LCD_Print("Clock  :");
	LCD_GoTo(1, 0);
	LCD_Print("Chrono :");
    while(1)
    {
    }
}
