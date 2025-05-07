#include "ALL_INIT.h"

void All_init()
{
	uart_R_start();
	key_start();

	LCD_GPIO_Init();
	LCD_Init();
	LCD_Fill(0, 0, 128, 180, WHITE);//ָ�����������ɫ

	pid_init(&servo1,POSITION_PID,10,0,0);
	pid_init(&servo2,POSITION_PID,10,0,0);

	servo_init(servo1_hadle);
	servo_init(servo2_hadle);
	chuankou_init();
	
}




