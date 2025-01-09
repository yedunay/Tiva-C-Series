#include "yedlib.h"
#include <math.h>

// Prototypes
void UART_Init(void);
void UART_SendString(const char *str);
void PerformFloatingPointOperations(void);

int main(void) {
    // Initialize UART for output
    UART_Init();

    // Perform floating-point calculations
    PerformFloatingPointOperations();

    // Infinite loop
    while (1) {}
}

void UART_Init(void) {
    // Enable clock for UART0 and Port A
    SYSCTL_RCGCUART_R |= (1 << 0);
    SYSCTL_RCGCGPIO_R |= (1 << 0);
    while ((SYSCTL_PRUART_R & (1 << 0)) == 0); // Wait for UART0 to be ready
    while ((SYSCTL_PRGPIO_R & (1 << 0)) == 0); // Wait for Port A to be ready

    // Configure PA0 and PA1 for UART
    GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);  // Enable alternate functions
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & ~0xFF) | 0x11; // UART function
    GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);    // Enable digital pins
    GPIO_PORTA_AMSEL_R &= ~((1 << 0) | (1 << 1)); // Disable analog functions

    // Configure UART0
    UART0_CTL_R &= ~(1 << 0);                   // Disable UART
    UART0_IBRD_R = 104;                         // Integer part of BRD
    UART0_FBRD_R = 11;                          // Fractional part of BRD
    UART0_LCRH_R = (0x3 << 5);                  // 8-bit, no parity, 1 stop bit
    UART0_CC_R = 0x0;                           // Use system clock
    UART0_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9); // Enable UART, RX, TX
}

void UART_SendString(const char *str) {
    while (*str) {
        while ((UART0_FR_R & (1 << 5)) != 0);   // Wait until TX FIFO is not full
        UART0_DR_R = *str;                      // Send character
        str++;
    }
}

void PerformFloatingPointOperations(void) {
    float a = 12.34;
    float b = 56.78;
    float addition = a + b;
    float subtraction = a - b;
    float multiplication = a * b;
    float division = a / b;
    float sine = sin(a);
    float cosine = cos(b);

    // Send results via UART
    UART_SendString("Floating Point Operations:\n");
    char buffer[64];

    // Addition
    sprintf(buffer, "Addition: %.2f\n", addition);
    UART_SendString(buffer);

    // Subtraction
    sprintf(buffer, "Subtraction: %.2f\n", subtraction);
    UART_SendString(buffer);

    // Multiplication
    sprintf(buffer, "Multiplication: %.2f\n", multiplication);
    UART_SendString(buffer);

    // Division
    sprintf(buffer, "Division: %.2f\n", division);
    UART_SendString(buffer);

    // Sine
    sprintf(buffer, "Sine: %.2f\n", sine);
    UART_SendString(buffer);

    // Cosine
    sprintf(buffer, "Cosine: %.2f\n", cosine);
    UART_SendString(buffer);
}
