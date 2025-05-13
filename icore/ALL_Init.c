#include "ALL_INIT.h"

void All_init()
{
	uart_R_start();
	key_start();

	LCD_GPIO_Init();
	
	LCD_Init();
	
	LCD_Fill(0, 0, 128, 160, WHITE);
	
	init_pid_parameters();

	servo_init(servo1_hadle);
	
	servo_init(servo2_hadle);
	
	chuankou_init();
	
	/*20ms中断*/
	HAL_TIM_Base_Start_IT(&htim6);
	
}




