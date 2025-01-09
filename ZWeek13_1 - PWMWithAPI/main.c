#include "yedlib.h"

// Initialize PWM using TivaWare APIs
void PWM_API_Init(void) {
    // Step 1: Enable peripherals for PWM and GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); // Enable PWM module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Enable GPIO Port B

    // Step 2: Configure GPIO pin for PWM output
    GPIOPinConfigure(GPIO_PB6_M0PWM0); // Configure PB6 as PWM0
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6); // Set PB6 as PWM pin

    // Step 3: Configure PWM clock
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // Set PWM clock divider to 64

    // Step 4: Configure PWM generator
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Step 5: Set PWM period and duty cycle
    uint32_t ui32PWMClock = SysCtlClockGet() / 64; // PWM clock frequency
    uint32_t ui32Period = (ui32PWMClock / 1000);   // Set 1 kHz frequency (1000 Hz)
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ui32Period); // Set period
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, ui32Period * 0.75); // Set duty cycle to 75%

    // Step 6: Enable PWM generator and output
    PWMGenEnable(PWM0_BASE, PWM_GEN_0); // Enable PWM generator 0
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true); // Enable PWM output on PB6
}

int main(void) {
    // Set system clock to 16 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Initialize PWM with APIs
    PWM_API_Init();

    while (1) {
        // PWM is running in the background
    }
}
