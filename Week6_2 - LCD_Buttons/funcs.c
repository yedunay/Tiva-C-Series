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

void keypad_init(void)
{
	SYSCTL_RCGCGPIO_R |= BIT_2; //port c
	SYSCTL_RCGCGPIO_R |= BIT_4; //port e

	GPIO_PORTE_DIR_R |= 0x0F;
	GPIO_PORTE_DEN_R |= 0x0F;
	GPIO_PORTE_ODR_R |= 0x0F;

	GPIO_PORTC_DIR_R &= ~0xF0; // Set PC4-7 inputs
	GPIO_PORTC_DEN_R |= 0xF0;
	GPIO_PORTC_PUR_R |= 0xF0;
}

unsigned char getKey(void)
{
	const unsigned char keymap[4][4]=
	{
	 {'1','2','3','A'},
	 {'4','5','6','B'},
	 {'7','8','9','C'},
	 {'*','0','#','D'},
	};
	uint8_t k_row = 0, k_col = 0;

	GPIO_PORTE_DATA_R = 0;
	k_col = GPIO_PORTC_DATA_R & 0xF0;
	if(k_col == 0xF0)
		return 0;//no key pressed
	while(1)
	{
		k_row = 0;
		GPIO_PORTE_DATA_R = 0x0E;
		delay_us(10);
		k_col = GPIO_PORTC_DATA_R & 0xF0;
		if(k_col != 0xF0)
			break;

		k_row = 1;
		GPIO_PORTE_DATA_R = 0x0D;
		delay_us(10);
		k_col = GPIO_PORTC_DATA_R & 0xF0;
		if(k_col != 0xF0)
			break;

		k_row = 2;
		GPIO_PORTE_DATA_R = 0x0B;
		delay_us(10);
		k_col = GPIO_PORTC_DATA_R & 0xF0;
		if(k_col != 0xF0)
			break;

		k_row = 3;
		GPIO_PORTE_DATA_R = 0x07;
		delay_us(10);
		k_col = GPIO_PORTC_DATA_R & 0xF0;
		if(k_col != 0xF0)
			break;
		return 0;
	}
	if(k_col == 0xE0)
		return keymap[k_row][0];
	if(k_col == 0xD0)
		return keymap[k_row][1];
	if(k_col == 0xB0)
		return keymap[k_row][2];
	if(k_col == 0x70)
		return keymap[k_row][3];
	return 0;
}
