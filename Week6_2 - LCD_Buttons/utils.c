
void delay_us(int sec)
{
    sec *= 3;
    while(sec--);
}

void delay_ms(int sec)
{
    sec *= 3180;
    while(sec--);
}

int strlen(char *str)
{
	int i = 0;
	while(str[i])
		i++;
	return i;
}
