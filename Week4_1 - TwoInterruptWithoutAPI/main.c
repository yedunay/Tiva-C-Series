#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#define PIN_0 (1 << 0)
#define PIN_1 (1 << 1)
#define PIN_2 (1 << 2)
#define PIN_3 (1 << 3)
#define PIN_4 (1 << 4)
#define PIN_5 (1 << 5)

#define BIT_30 (1 << 30)

void init(void)
{
    SYSCTL_RCGCGPIO_R |= PIN_5; // For portf clock signal is active
    while ((SYSCTL_PRGPIO_R & PIN_5) == 0); // Wait till clock's active station

    GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock the GPIOPORTF
    GPIO_PORTF_CR_R |= (PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4); // Make those pins configurable

    GPIO_PORTF_AFSEL_R &= ~(PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4); // Close the alternative functions for those pins

    GPIO_PORTF_DIR_R |= (PIN_1 | PIN_2 | PIN_3); // Set outputs
    GPIO_PORTF_DIR_R &= ~(PIN_0 | PIN_4); // Set inputs

    GPIO_PORTF_DEN_R |= (PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4); // Portf set those pins digital enable
    GPIO_PORTF_PUR_R |= (PIN_0 | PIN_4); // Set pull-up res for pf0 & pf4

    GPIO_PORTF_IS_R &= ~(PIN_4 | PIN_1); // PF4 edge interrupt
    GPIO_PORTF_IBE_R &= ~(PIN_4 | PIN_1); // PF1 and PF4 only one edge interrupt
    GPIO_PORTF_IEV_R &= ~(PIN_4 | PIN_1); // Edge interrupt (When the button is clicked)
    GPIO_PORTF_IM_R |= (PIN_4 | PIN_1); // PF4 PF1 activate interrupt.

    //Nested Vectored Interrupt Controller
    NVIC_EN0_R |= (BIT_30); // Enables the IRQ30 (Portf interrupt)
}

void delay(int i)
{
    while(i--);
}

/*
 * TO DEFINE THE PORTF INTERRUPT FUNCTION,
 * we need to declare the function in the startup_ccs.c file.
 *
 * External declarations for the interrupt handlers used by the application:
 *     extern void GPIOF_Handler(void);
 *
 * Then, add the following to the vector table:
 *
 *     void (* const g_pfnVectors[])(void) =
 *     {
 *         (void (*)(void))((uint32_t)&__STACK_TOP),
 *         GPIOF_Handler,                      // GPIO Port F
 *     };
 */
void GPIOF_Handler(void)
{
    if (GPIO_PORTF_RIS_R & PIN_4)
    {
        GPIO_PORTF_ICR_R = PIN_4; // Clear the interrupt flag for PF4
        GPIO_PORTF_DATA_R |= PIN_1; // Turn on the LED connected to PF1
        delay(1000000); // Delay
        GPIO_PORTF_DATA_R &= ~PIN_1; // Turn off the LED connected to PF1
        delay(1000000); // Delay
    }
    else if (GPIO_PORTF_RIS_R & PIN_0)
    {
        GPIO_PORTF_ICR_R = PIN_0; // Clear the interrupt flag for PF4
        GPIO_PORTF_DATA_R |= PIN_2; // Turn on the LED connected to PF2
        delay(1000000); // Delay
        GPIO_PORTF_DATA_R &= ~PIN_2; // Turn off the LED connected to PF2
        delay(1000000); // Delay
    }
}

int main(void)
{
    init();

    while(1)
    {
        // Null statement
    }
}