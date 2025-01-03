#include <yedlib.h>

void delay_us(int time)
{
    SysCtlDelay((time * (SysCtlClockGet() / 3)) / 1000000);
}

void delay_ms(int time)
{
    SysCtlDelay((time * (SysCtlClockGet() / 3)) / 1000);
}
