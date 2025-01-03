#include "yedlib.h"

void LCD_init(void)
{
    SYSCTL_RCGCGPIO_R |= (BIT_0 | BIT_1); // Enable the clock for PA & PB

    GPIO_PORTA_DIR_R |=  (BIT_7 | BIT_6 | BIT_5); //PORTA CONTROLS RS,E,RW
    GPIO_PORTA_DEN_R |=  (BIT_7 | BIT_6 | BIT_5);

    GPIO_PORTB_DIR_R |= 0xFF;
    GPIO_PORTB_DEN_R |= 0xFF;

    LCD_Cmd(BIT_3 | BIT_4 | BIT_5); // 8 bits , 2 line display , 5x7 Font
    LCD_Cmd(BIT_1 | BIT_2); // Increment automatically
    LCD_Cmd(BIT_0 | BIT_1 | BIT_2 | BIT_3); // Turn on display
    LCD_Cmd(BIT_0); // Clear display
}

void LCD_Data(unsigned char data)
{
    GPIO_PORTA_DATA_R = (BIT_5); // RS = 1, E = 0, RW = 0
    GPIO_PORTB_DATA_R = (data);
    GPIO_PORTA_DATA_R |= (BIT_7); //
    delay_us(100);
    GPIO_PORTA_DATA_R = 0;
    delay_us(100);
}

void LCD_Cmd(unsigned char cmd)
{
    GPIO_PORTA_DATA_R = 0; // RS = 0, RW = 0, E = 0
    GPIO_PORTB_DATA_R = cmd;
    GPIO_PORTA_DATA_R = BIT_7; //E = 1 to secure command
    delay_us(100);
    GPIO_PORTA_DATA_R = 0;
    if (cmd < 4) // For cursor pos change or screen clear
        delay_ms(2);
    else
        delay_us(37);
}
