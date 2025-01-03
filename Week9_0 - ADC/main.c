#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "yedlib.h"

void SetInitSettings();
void DisplayADCValueOnLCD(uint32_t adcValue);

uint32_t value[1]; // The value read from ADC will be stored here

// ADC interrupt service routine
void ADC0Handler(void) {
    // Clear the ADC interrupt flag
    ADCIntClear(ADC0_BASE, 3);

    // Get the ADC result
    ADCSequenceDataGet(ADC0_BASE, 3, value);

    // Write the ADC data to the LCD
    DisplayADCValueOnLCD(value[0]);
}

// A simple formatting function instead of snprintf
void DisplayADCValueOnLCD(uint32_t adcValue) {
    char buffer[5]; // Buffer to print to the LCD
    int intVoltage = (adcValue * 3300) / 4096; // Calculate the voltage in mV
    memset(buffer, 0, 5);

    buffer[0] = (intVoltage / 1000) + '0'; // Integer part of the voltage
    buffer[1] = '.';
    buffer[2] = ((intVoltage / 100) % 10) + '0'; // First decimal place of the voltage
    buffer[3] = ((intVoltage / 10) % 10) + '0';  // Second decimal place of the voltage
    buffer[4] = 'V';
    // Print to the LCD
    LCD_GoTo(1, 0);
    LCD_Print(buffer);
}

int main(void) {
    // Perform initial settings
    SetInitSettings();

    while (1) {
        // Main loop
    }
}

void SetInitSettings() {
    // Set system clock (50 MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    LCD_Init();
    LCD_Cmd(GPIO_PIN_0);
    LCD_Cmd(GPIO_PIN_7);
    delay_ms(100);

    // Enable peripherals for GPIOF and ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Configure Timer0
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // Set Timer0 to periodic mode

    // Load value for Timer 0A (e.g., trigger every 1 second)
    uint32_t period = SysCtlClockGet();  // 50 MHz corresponds to 1 second
    TimerLoadSet(TIMER0_BASE, TIMER_A, period - 1);

    // Enable Timer0A
    TimerControlTrigger(TIMER0_BASE, TIMER_A, true); // Enable timer for ADC triggering
    TimerEnable(TIMER0_BASE, TIMER_A); // Start Timer 0A

    // Configure ADC0 Sequence 3 (triggered by Timer)
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);

    // Steps for Sequence 3 (1 sample on CH0)
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

    // Enable Sequence 3
    ADCSequenceEnable(ADC0_BASE, 3);

    // Enable ADC interrupts
    IntMasterEnable();
    ADCIntRegister(ADC0_BASE, 3, ADC0Handler);
    ADCIntEnable(ADC0_BASE, 3);
}
 