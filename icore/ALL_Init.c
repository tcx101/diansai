#include "ALL_INIT.h"

void All_init()
{
	uart_R_start();
	key_start();
	LCD_GPIO_Init();
	LCD_Init();
	LCD_Fill(0, 0, 128, 180, WHITE);//指定区域填充颜色
}




