#include "yedlib.h"

void LCD_Print(const char *str)
{
	while(*str)
		LCD_Data(*str++);
}

void LCD_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //Configure control pins (RS, RW, E) for LCD on PORTA and data pins (D0-D7) for LCD on PORTB
    GPIOPinTypeGPIOOutput(LCD_CTRL_PORT, LCD_RS | LCD_RW | LCD_E);
    GPIOPinTypeGPIOOutput(LCD_DATA_PORT, 0xFF);

    LCD_Cmd(0x38); // Set 8-bit data, 2-line display, 5x7 font
    delay_ms(10);
    LCD_Cmd(0x06); // Set automatic increment (cursor moves to the right)
    delay_ms(10);
    LCD_Cmd(0x0F); // Turn on display, show cursor, make cursor blink
    delay_ms(10);
    LCD_Cmd(0x01); // Clear the display
    delay_ms(10);
}

void LCD_Data(unsigned char data)
{
    GPIOPinWrite(LCD_CTRL_PORT, LCD_RS | LCD_RW | LCD_E, LCD_RS); // RS = 1 Data mode
    delay_ms(10);

    GPIOPinWrite(LCD_DATA_PORT, 0xFF, data); // Send the data to the data port (PORTB)
    delay_ms(10);

    GPIOPinWrite(LCD_CTRL_PORT, LCD_E, LCD_E); // Set E = 1 to latch the data into LCD, then reset it
    delay_us(10);
    GPIOPinWrite(LCD_CTRL_PORT, LCD_E, 0);
    delay_us(10);
}

void LCD_Cmd(unsigned char cmd)
{
    GPIOPinWrite(LCD_CTRL_PORT, LCD_RS | LCD_RW | LCD_E, 0); // RS = 0 Command mode
    delay_ms(10);

    GPIOPinWrite(LCD_DATA_PORT, 0xFF, cmd);     // Send the command to the data port (PORTB)
    delay_ms(10);

    GPIOPinWrite(LCD_CTRL_PORT, LCD_E, LCD_E);  // Set E = 1 to latch the command into LCD, then reset it
    delay_us(1);
    GPIOPinWrite(LCD_CTRL_PORT, LCD_E, 0);

    if (cmd < 4)
        delay_ms(2);  // Longer delay for commands like clear display
    else
        delay_us(37);
}

void LCD_GoTo(uint8_t row, uint8_t col)
{
	unsigned char pos;
	if (row == 0)
		pos = 0x80 + col;
	else if (row == 1)
		pos = 0xC0 + col;
	LCD_Cmd(pos);
}
