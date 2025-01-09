#include "yedlib.h"

void ADC_Init(void) {
    SYSCTL_RCGCADC_R |= (1 << 0); // Enable ADC0 clock
    SYSCTL_RCGCGPIO_R |= (1 << 4); // Enable GPIOE clock

    GPIO_PORTE_AFSEL_R |= (1 << 3);  // Enable alternate function on PE3
    GPIO_PORTE_DEN_R &= ~(1 << 3);  // Disable digital function on PE3
    GPIO_PORTE_AMSEL_R |= (1 << 3); // Enable analog function on PE3

    ADC0_ACTSS_R &= ~(1 << 3);       // Disable sequencer 3
    ADC0_EMUX_R = (ADC0_EMUX_R & ~0xF000) | 0x0000; // Set sequencer 3 to processor trigger
    ADC0_SSMUX3_R = 0;              // Set channel 0 (AIN0 - PE3)
    ADC0_SSCTL3_R = 0x06;           // Enable IE and END bits
    ADC0_ACTSS_R |= (1 << 3);       // Enable sequencer 3
}

int ADC_Read(void) {
    ADC0_PSSI_R = (1 << 3);         // Start conversion
    while ((ADC0_RIS_R & (1 << 3)) == 0); // Wait for conversion to complete
    int result = ADC0_SSFIFO3_R & 0xFFF; // Read 12-bit result
    ADC0_ISC_R = (1 << 3);          // Clear interrupt flag
    return result;
}

void PWM_UpdateDutyCycle(uint16_t dutyCycle) {
    PWM0_0_CMPA_R = ((16000 * dutyCycle) / 100) - 1; // Update compare value
}

int main(void) {
    PWM_Init();
    ADC_Init();

    while (1) {
        uint16_t adcValue = ADC_Read();     // Read ADC value (0-4095)
        uint16_t dutyCycle = (adcValue * 100) / 4095; // Convert to duty cycle percentage (0-100%)
        PWM_UpdateDutyCycle(dutyCycle);    // Update PWM duty cycle
    }
}
