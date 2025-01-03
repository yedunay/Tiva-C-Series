#include "yedlib.h"

// Configure Sequencer 3
void SequencerSet()
{
    ADC0_ACTSS_R &= ~(1 << 3); // Disable Sequencer 3
    ADC0_EMUX_R = (ADC0_EMUX_R & ~0xF000) | 0x5000; // Timer trigger (0x5) - Software trigger (0x0)
    ADC0_SSMUX3_R = 0x0; // Sequencer 3 will sample channel 0 (AIN0 - PE3)
    ADC0_SSCTL3_R = 0x06 | (1 << 3); // Set IE (Interrupt Enable) and END (End Step) bits
    ADC0_IM_R |= 0x08; // Enable interrupt mask
    ADC0_ACTSS_R |= 0x08; // Enable Sequencer 3
}

// Initial settings
void SetInitSettings()
{
    // 1. Enable ADC clock
    SYSCTL_RCGCADC_R |= (1 << 0); // Enable clock for ADC0
    while ((SYSCTL_PRADC_R & (1 << 0)) == 0); // Wait until ADC is ready

    // 2. Enable GPIO clock
    SYSCTL_RCGCGPIO_R |= (1 << 4); // Enable clock for Port E
    while ((SYSCTL_PRGPIO_R & (1 << 4)) == 0); // Wait until GPIO is ready

    // 3. Configure PE3 pin for ADC
    GPIO_PORTE_AFSEL_R |= (1 << 3);  // Set PE3 to alternate function mode
    GPIO_PORTE_DEN_R &= ~(1 << 3);  // Disable digital function on PE3
    GPIO_PORTE_AMSEL_R |= (1 << 3); // Enable analog function on PE3

    // 4. Configure Sequencer
    SequencerSet();
}

// ADC read function
int ADC_Read(void)
{
    ADC0_PSSI_R = 0x08; // Start Sequencer 3
    while ((ADC0_RIS_R & 0x08) == 0); // Wait until conversion is complete
    int result = ADC0_SSFIFO3_R & 0xFFF; // Get ADC result (12-bit)
    ADC0_ISC_R = 0x08; // Clear interrupt flag
    return result; // Return ADC result
}

int main(void) {
    SetInitSettings(); // Perform initial settings

    volatile int adc_value = 0; // Value read from ADC
    while (1) {
        adc_value = ADC_Read(); // Read ADC value
        // You can use adc_value (e.g., to display on an LCD or process it)
    }
}
