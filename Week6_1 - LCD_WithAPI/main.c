#include <yedlib.h>

static void lcd_print(char *str, int row)
{
	row == 2 ? LCD_Cmd(GPIO_PIN_7 | GPIO_PIN_6) : LCD_Cmd(GPIO_PIN_7);

	char *p1 = str;
	int i, len = strlen(str);

	for(i = 0; i < len; i++) //Row-1
	{
	    LCD_Data(*p1++);
	    delay_ms(100);
	}
}


int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    LCD_init();
    LCD_Cmd(GPIO_PIN_0); // Clear the screen
    LCD_Cmd(GPIO_PIN_7); // Set the cursor home
    delay_ms(500);

    while(1)
    {
    	LCD_Cmd(GPIO_PIN_0); // Clear the screen
    	lcd_print("OSMAN SELIM CIL", 1);
    	delay_ms(500);
    	lcd_print("BIR DUNYA ARMUDU", 2);
    	delay_ms(1000);
    	LCD_Cmd(GPIO_PIN_0); // Clear the screen
    	lcd_print("O KI DUNYANIN EN", 1);
    	delay_ms(500);
    	lcd_print("IYI ARMUDUDUR !", 2);
    }
}
