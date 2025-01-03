#ifndef YEDLIB_H
#define YEDLIB_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"

#define LCD_RS GPIO_PIN_5
#define LCD_RW GPIO_PIN_6
#define LCD_E  GPIO_PIN_7
#define LCD_DATA_PORT GPIO_PORTB_BASE
#define LCD_CTRL_PORT GPIO_PORTA_BASE

void delay_ms(int sec);
void delay_us(int sec);
int strlen(char *str);
void LCD_init(void);
void LCD_Cmd(unsigned char cmd);
void LCD_Data(unsigned char data);

#endif
