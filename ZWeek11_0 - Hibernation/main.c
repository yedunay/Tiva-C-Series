#include "yedlib.h"

// Prototypes
void Hibernation_Init(uint32_t wakeTimeSeconds);
void Enter_Hibernation(void);
void GPIO_Init(void);

int main(void) {
    // Initialize GPIO (for example, to show a status with an LED)
    GPIO_Init();

    // Initialize Hibernation module to wake up after 10 seconds
    Hibernation_Init(10);

    // Enter Hibernation mode
    Enter_Hibernation();

    // Actions after waking up from hibernation
    while (1) {
        // GPIO action: Toggle an LED to indicate the system is awake
        GPIO_PORTF_DATA_R ^= (1 << 2); // Toggle PF2 (Blue LED)
        for (volatile int i = 0; i < 1000000; i++); // Delay
    }
}

void GPIO_Init(void) {
    // Enable clock for Port F
    SYSCTL_RCGCGPIO_R |= (1 << 5);
    while ((SYSCTL_PRGPIO_R & (1 << 5)) == 0); // Wait for Port F to be ready

    // Configure PF2 (Blue LED) as output
    GPIO_PORTF_DIR_R |= (1 << 2);
    GPIO_PORTF_DEN_R |= (1 << 2);
}

void Hibernation_Init(uint32_t wakeTimeSeconds) {
    // Enable clock for Hibernation module
    SYSCTL_RCGCHIB_R |= (1 << 0);
    while ((SYSCTL_PRHIB_R & (1 << 0)) == 0); // Wait for Hibernation module to be ready

    // Disable Hibernation module before configuration
    HIB_CTL_R &= ~(1 << 0);

    // Set the RTC match value for wake-up
    HIB_RTCC_R = 0;                          // Reset RTC
    HIB_RTCM0_R = wakeTimeSeconds;           // Set match value for wake-up

    // Enable the RTC, enable match interrupt, and enable Hibernation
    HIB_CTL_R = (1 << 1) | (1 << 3) | (1 << 0);

    // Wait for the Hibernation module to be ready
    while ((HIB_CTL_R & (1 << 0)) == 0);
}

void Enter_Hibernation(void) {
    // Request Hibernation
    HIB_CTL_R |= (1 << 2);
    while (1); // Wait until the system enters Hibernation
}
