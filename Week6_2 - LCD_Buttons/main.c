#include <yedlib.h>

static void init(void)
{
	keypad_init();
    LCD_init();
    LCD_Cmd(BIT_0); // Clear the screen
    LCD_Cmd(BIT_7); // Set the cursor home
    delay_ms(500);
}

int main(void)
{
	unsigned char key;
	int key_cnt = 0;

	init();
    while(1)
    {
    	key = getKey();
    	delay_ms(120);
    	if (key)
    	{
    		key_cnt++;
    		if (key == '*')
    		{
    			LCD_Cmd(BIT_0); // Clear the screen
    			key_cnt = 0;
    		}
    		else if (key_cnt > 16)
    		{
    			LCD_Cmd(BIT_6 | BIT_7); // 2nd Row
    			key_cnt = 0;
    			LCD_Data(key);
    		}
    		else
    			LCD_Data(key);

    	}
    }
}
