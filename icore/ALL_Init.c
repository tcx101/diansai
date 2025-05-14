#include "ALL_INIT.h"

void All_init()
{
	uart_R_start();
	key_start();

	LCD_GPIO_Init();
	
	LCD_Init();
	
	LCD_Fill(0, 0, 128, 160, WHITE);
<<<<<<< HEAD

	pid_init(&servo1,POSITION_PID,1,0.1,0.5);
	
	pid_init(&servo2,POSITION_PID,0.8,0.1,0.4);
=======
	
	init_pid_parameters();
>>>>>>> 04fc011eb2db7265daeae95812488f8de14f1d80

	servo_init(servo1_hadle,servo1_mid);
	
	servo_init(servo2_hadle,servo2_mid);
	
	chuankou_init();
	
	/*20ms中断*/
	//HAL_TIM_Base_Start_IT(&htim6);
	
}




