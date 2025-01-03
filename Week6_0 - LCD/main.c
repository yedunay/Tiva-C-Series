#include <yedlib.h>

static void lcd_print(char *str, int row)
{
	row == 2 ? LCD_Cmd(BIT_7 | BIT_6) : LCD_Cmd(BIT_7);

	char *p1 = str;
	int i, len = strlen(str);

	for(i = 0; i < len; i++) //Row-1
	{
	    LCD_Data(*p1++);
	    delay_ms(1);
	}
}

int main(void)
{
    LCD_init();
    LCD_Cmd(BIT_0); // Clear the screen
    LCD_Cmd(BIT_7); // Set the cursor home
    delay_ms(500);
    lcd_print("YUNUS EMRE DUNAY", 1);
    lcd_print("MICROCONTROLLERS", 2);
}
