#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"     // For GPIO_PIN_x definitions
#include "inc/hw_memmap.h"         // For hardware memory addresses
#include "inc/hw_types.h"          // For data types
#include "inc/hw_gpio.h"

void PortF_Init(void);
void GPIOF_Handler(void);
void delay(int time);

int main(void) {
    PortF_Init(); // Initialize Port F

    while(1) {
        // Do nothing in the main loop, waiting for interrupts
    }
}

void PortF_Init(void) {
    // Enable the clock signal for Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {} // Wait until the clock is ready

    // Unlock PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // Configure PF0, PF1, PF2, PF3, PF4 pins
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Enable pull-up resistors for PF0 and PF4
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure edge interrupts for PF0 and PF4, falling edge (when button is pressed)
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    // Enable global interrupts
    IntEnable(INT_GPIOF);
    IntMasterEnable();
}

void GPIOF_Handler(void) {
    uint32_t status = GPIOIntStatus(GPIO_PORTF_BASE, true);
    GPIOIntClear(GPIO_PORTF_BASE, status); // Clear the interrupt flag

    if (status & GPIO_PIN_4) { // If PF4 interrupt occurred
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // Turn on PF1 LED (red LED)
        delay(100000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // Turn off PF1 LED
        delay(100000);
    } else if (status & GPIO_PIN_0) { // If PF0 interrupt occurred
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // Turn on PF2 LED (blue LED)
        delay(100000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // Turn off PF2 LED
        delay(100000);
    }
}

void delay(int time) {
    while(time--);
}

