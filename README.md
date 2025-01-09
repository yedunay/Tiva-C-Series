### **Tiva C Series Weekly Learning Repository** README

---

Welcome to the **Tiva C Series Weekly Learning Repository**! This repository is designed for individuals aiming to master the Tiva C Series microcontroller step-by-step. Each week focuses on a specific topic, including examples and detailed explanations to help you learn efficiently.

---

## Weekly Topics Overview

### Week 1: Introduction to Microcontrollers
- Overview of microcontrollers and their applications.
- Introduction to the Tiva C Series.
- Setting up the development environment.

### Week 2: GPIO (General Purpose Input/Output)
- Basics of GPIO.
- Configuring GPIO pins for input and output.
- Practical examples of LED control.

### Week 3: Buttons and Button Operations
- Configuring GPIO pins for button inputs.
- Implementing debounce logic.
- Example projects using buttons.

### Week 4: Interrupts
- Understanding interrupts and their importance.
- Configuring and handling GPIO interrupts.
- Real-world examples using interrupts.

### Week 6: LCD Applications (with and without APIs)
- Interfacing LCDs with Tiva C Series.
- Coding examples with APIs and without APIs.
- 4x4 button matrix with LCD applications.

### Week 7: Timers
- Configuring and using timers.
- Generating delays and periodic interrupts.
- Example applications using timers.

### Week 8: Serial Port Operations
- Introduction to UART (Universal Asynchronous Receiver/Transmitter).
- Configuring and using serial communication.
- Example projects with UART.

### Week 9: ADC (Analog-to-Digital Conversion)
- Introduction to ADC and its applications.
- Configuring ADC modules on Tiva C Series.
- Example applications for sensor data acquisition.

### Week 10: ADC Without APIs
- Implementing ADC functionality without APIs.
- Low-level register operations for ADC.
- Hands-on projects using ADC.

### Week 11: Hibernation Module
- Introduction to the Hibernation Module in Tiva C Series.
- Configuring the Hibernation Module for low-power operation.
- Example project: System wake-up after a preset time.

### Week 12: Floating Point Operations
- Understanding floating-point operations on the Tiva C Series.
- Arithmetic and trigonometric calculations.
- Example project: UART-based output of floating-point calculations.

---

## Repository Structure
The repository is organized as follows:
- **Week1/**: Contains code and resources for the first week.
- **Week2/**: GPIO-related examples and documentation.
- **Week3/**: Button-related projects and explanations.
- **Week4/**: Interrupt examples and use cases.
- **Week6/**: LCD interfacing and 4x4 button applications.
- **Week7/**: Timer examples and projects.
- **Week8/**: UART-based serial communication.
- **Week9/**: ADC basics and examples.
- **Week10/**: ADC implementation without APIs.
- **Week11/**: Hibernation module examples and use cases.
- **Week12/**: Floating-point operations with practical examples.

---

## Week 11: Hibernation Module
### **Description:**
The hibernation module is used to put the microcontroller into a low-power state and wake it up after a defined time. 

### **Key Features:**
- Low-power sleep mode for energy-saving applications.
- RTC-based wake-up configuration.

### **Example Project:**
- Code demonstrates using the Hibernation module to wake up after 10 seconds and toggle an LED.

---

## Week 12: Floating Point Operations
### **Description:**
This week explores how to perform floating-point arithmetic and trigonometric calculations on the Tiva C Series.

### **Key Features:**
- Addition, subtraction, multiplication, and division of floating-point numbers.
- Calculation of sine and cosine values.
- UART-based display of floating-point results.

---

## Getting Started
1. Clone the repository:
   ```bash
   git clone https://github.com/yedunay/Tiva-C-Series.git
   ```
2. Navigate to the desired week's folder.
3. Open the project files in your preferred IDE (e.g., Code Composer Studio).
4. Follow the instructions in the README file within each week's folder.

---

## Prerequisites
- **Hardware**: Tiva C Series LaunchPad (TM4C123G or similar).
- **Software**: Code Composer Studio or another compatible IDE.

---
