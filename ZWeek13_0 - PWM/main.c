#include "yedlib.h"

// Configure PWM for a basic signal
void PWM_Init(void) {
    SYSCTL_RCGCPWM_R |= (1 << 0);  // Enable PWM module 0 clock
    SYSCTL_RCGCGPIO_R |= (1 << 1); // Enable GPIOB clock

    GPIO_PORTB_AFSEL_R |= (1 << 6); // Enable alternate function on PB6
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~0x0F000000) | 0x04000000; // Set PB6 as PWM
    GPIO_PORTB_DEN_R |= (1 << 6);   // Enable digital function on PB6

    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x001E0000) | (0x000A0000); // Set PWM clock divider to 64

    PWM0_0_CTL_R = 0;               // Disable generator 0 during configuration
    PWM0_0_GENA_R = 0x0000008C;     // Set output high on load, low on CMPA
    PWM0_0_LOAD_R = 16000 - 1;      // Set load value for 1kHz frequency (16MHz / 64 / 1kHz)
    PWM0_0_CMPA_R = 12000 - 1;      // Set compare value for 75% duty cycle
    PWM0_0_CTL_R = 1;               // Enable generator 0
    PWM0_ENABLE_R |= (1 << 0);      // Enable PWM0 output on PB6
}

int main(void) {
    PWM_Init();
    while (1) {
        // PWM is running
    }
}
