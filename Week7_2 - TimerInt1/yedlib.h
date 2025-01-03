#ifndef YEDLIB_H
#define YEDLIB_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"

#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_3 (1 << 3)
#define BIT_4 (1 << 4)
#define BIT_5 (1 << 5)
#define BIT_6 (1 << 6)
#define BIT_7 (1 << 7)

#define LED_RED BIT_1
#define LED_BLUE BIT_2
#define LED_GREEN BIT_3

// Overflow counter variables
volatile uint32_t timerACounter = 0;
volatile uint32_t timerBCounter = 0;

// Overflow limits
#define TIMER_A_OVERFLOW_LIMIT 400          // Limit for Timer A overflows
#define TIMER_B_OVERFLOW_LIMIT 800          // Limit for Timer B overflows

void SetInitSettings();                     // Function to initialize settings
void timerAInterruptHandler();               // Interrupt handler for Timer A
void timerBInterruptHandler();               // Interrupt handler for Timer B


#endif
