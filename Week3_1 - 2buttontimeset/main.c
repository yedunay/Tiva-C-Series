#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

void init(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_6MA, GPIO_PIN_TYPE_STD_WPU);
}

void led_blinker(int pin_val, uint32_t delay_time)
{
    GPIOPinWrite(GPIO_PORTF_BASE, pin_val, pin_val);
    SysCtlDelay(delay_time);
    GPIOPinWrite(GPIO_PORTF_BASE, pin_val, !pin_val);
    SysCtlDelay(delay_time);
}

void color_change(int cnt, uint32_t delay_time)
{
    cnt %= 3;
    switch (cnt)
    {
    case 0:
        led_blinker(GPIO_PIN_1, delay_time);
        break;
    case 1:
        led_blinker(GPIO_PIN_2, delay_time);
        break;
    case 2:
        led_blinker(GPIO_PIN_3, delay_time);
        break;
    }
}

int main(void)
{
    init();
    uint32_t delay_time = 1000000;
    int cnt = 0;
    while (1)
    {
        if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
            delay_time += 100000;
        else if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
        {
            if(delay_time > 100000)
                delay_time -= 100000;
            else
                delay_time = 1;
        }
        color_change(cnt, delay_time);
        cnt++;
    }
}
