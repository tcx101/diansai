#include "ISR.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*20msÖÐ¶Ï*/
	if(htim->Instance==TIM6)
	{
		if(point_data.light[0]!=0 &&point_data.light[1]!=0)
		{PID_servo1();
		
		PID_servo2();}
	}
	
	/*10msÖÐ¶Ï*/
		if(htim->Instance==TIM7)
	{
		key_scan();
		
	}
	
}

