#include "inc/tm4c123gh6pm.h"
#include <stdint.h>

// ####MANUAL DECLARATIONS
//#define RCGCGPIO (*((unsigned int *)0x400FE608U))
//#define GPIOF_BASE (0x40025000U)
//#define GPIOF_DIR (*((unsigned int *)(GPIOF_BASE + 0x400U)))
//#define GPIOF_DEN (*((unsigned int *)(GPIOF_BASE + 0x51CU)))
//#define GPIOF_DATA (*((unsigned int *)(GPIOF_BASE + 0x3FCU)))
////GPIO_PORTF_DATA_R = (*((volatile uint32_t *)0x400253FC))
//
//int main(void)
//{
//    RCGCGPIO = 0x20U;//GPIOF BLOCK ON
//    GPIOF_DIR = 0x0EU;//GPIOF PIN DIRECTION REG
//    GPIOF_DEN = 0x0EU;//DIGITAL FUNC REGISTER
//    while(1)
//    {
//        GPIOF_DATA = 0x02U;//IN GPIO DATA REGISTER LED ON
//        int i = 1000000;
//        while(i--);
//        GPIOF_DATA = 0x00U;//IN GPIO DATA REGISTER LED ON
//        i = 1000000;
//        while(i--);
//    }
//
//}

#include "FuncsandDefines.h"
int main(void)
  {
    SYSCTL_RCGCGPIO_R |= BIT_5;
    SYSCTL_GPIOHBCTL_R |= BIT_5;
    GPIO_PORTF_AHB_DIR_R |= (LED_BLUE | LED_GREEN | LED_RED);
    GPIO_PORTF_AHB_DEN_R |= (LED_BLUE | LED_GREEN | LED_RED);
    int lim = 5;
    while(lim--)
    {
        GPIO_PORTF_AHB_DATA_R |= LED_RED;
        delay(1e6);
        GPIO_PORTF_AHB_DATA_R &= ~LED_RED;
        GPIO_PORTF_AHB_DATA_R |= LED_BLUE;
        delay(1e6);
        GPIO_PORTF_AHB_DATA_R &= ~LED_BLUE;
        GPIO_PORTF_AHB_DATA_R |= LED_GREEN;
        delay(1e6);
        GPIO_PORTF_AHB_DATA_R &= ~LED_GREEN;
        delay(1e6);
    }
}
