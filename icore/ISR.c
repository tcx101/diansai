#include "ISR.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*20ms�ж�*/
	if(htim->Instance==TIM6)
	{
		PID_servo1();
		
		PID_servo2();
	}
	
	/*10ms�ж�*/
		if(htim->Instance==TIM7)
	{
		key_scan();
		
	}
	
}

