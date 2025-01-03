#ifndef YEDLIB_H
#define YEDLIB_H

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_3 (1 << 3)
#define BIT_4 (1 << 4)
#define BIT_5 (1 << 5)
#define BIT_6 (1 << 6)
#define BIT_7 (1 << 7)

#define LED_RED BIT_1
#define LED_BLUE BIT_2
#define LED_GREEN BIT_3

void delay_ms(int sec);
void delay_us(int sec);
void LCD_init(void);
void LCD_Cmd(unsigned char cmd);
void LCD_Data(unsigned char data);

#endif
