#include <yedlib.h>

int main(void)
{
    LCD_init();
    LCD_Cmd(BIT_0); // Clear the screen
    LCD_Cmd(BIT_7); // Set the cursor home
    delay_ms(500);
    char row1[100] = "Yunus Emre Dunay";
    char row2[100] = "Microcontrollers";
    char *p1 = row1;
    char *p2 = row2;
    int i;

    for(i = 0; i < 16; i++) //Row-1
    {
        LCD_Data(*p1++);
        delay_ms(1);
    }

    LCD_Cmd(BIT_7 | BIT_6);  // Set the cursor 2. row
    delay_ms(500);

    for(i = 0; i < 16; i++) //Row-1
    {
        LCD_Data(*p2++);
        delay_ms(1);
    }
}
