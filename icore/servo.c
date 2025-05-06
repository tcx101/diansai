#include  "servo.h"

void set_servo(TIM_HandleTypeDef handle,uint16_t compare)
{
	if(compare<=500)
	{compare=500;}
	else if (compare>=2500)
	{compare=2500;}
	
  	__HAL_TIM_SetCompare(&handle,TIM_CHANNEL_1,compare);
}

void servo_init(TIM_HandleTypeDef handle)
{
	HAL_TIM_PWM_Start(&handle,TIM_CHANNEL_1);
	
	__HAL_TIM_SetCompare(&handle,TIM_CHANNEL_1,mid_value);
	
	
}




