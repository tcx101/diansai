#include "headfile.h"
key_str key[4];
void key_scan()
{
	uint8_t i;
	key[0].key_level=HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin);
	key[1].key_level=HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin);
	key[2].key_level=HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin);
	key[3].key_level=HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin);
	for(i=0;i<=3;i++)
	{
		if(key[i].key_state==0)
		{
			if(key[i].key_level==0)
			{
				key[i].key_state++;
				//长按键
				//key[i].key_time=0;
				break;
			}

		}
		
		else if(key[i].key_state==1)
		{
			if(key[i].key_level==0)
			{
				key[i].key_state++;
				break;
			}
			else{key[i].key_state=0;break;}
		}
		
		else if(key[i].key_state==2)
		{
			if(key[i].key_level==0)
			{
				/*长按键模式*/
				//key[i].key_time++;
        //break;				
			}
		else
		 {
			 /*短按键模式*/
			 key[i].key_state=0;
			 key[i].key_flag=1;
			 break;
		 /*长按键模式*/
//			 if(key[i].key_time>=50)
//			 {key[i].key_longflag=1;
//				key[i].key_state=0; 
//			 break;}
//			 
//			 else
//			 {key[i].key_flag=1;
//				key[i].key_state=0;
//			 break;}	 
			}
		}
	}
}

void key_start()
{
	HAL_TIM_Base_Start_IT(&htim7);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM7)
	{
		key_scan();
		
	}
}
void key_function()
{
	if(key[0].key_flag==1)
	{	static char str[20];
		sprintf(str,"aa");
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)str,strlen(str));
		key[0].key_flag=0;
		flash_set_buffer_PARA();
	}
	
}


