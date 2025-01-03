#include <yedlib.h>

volatile int sec = 0;
volatile int min = 0;
volatile int hour = 0;

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
}
