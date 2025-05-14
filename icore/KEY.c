#include "headfile.h"
key_str key[4];
uint8_t page=1;
uint8_t mode=1;
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
				//按下按键
				key[i].key_time=0;
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
				key[i].key_time++;
        break;				
			}
		else
		 {
//			 /*短按键模式*/
//			 key[i].key_state=0;
//			 key[i].key_flag=1;
//			 break;
		 /*长按键模式*/
			 if(key[i].key_time>=50)
			 {key[i].key_longflag=1;
				key[i].key_state=0; 
			 break;}
			 
			 else
			 {key[i].key_flag=1;
				key[i].key_state=0;
			 break;}	 
			}
		}
	}
}

void key_start()
{
	HAL_TIM_Base_Start_IT(&htim7);
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance==TIM7)
//	{
//		key_scan();
//		
//	}
//}
void key_function()
{
	
	if(page==1)
	{if(key[0].key_flag==1)
	{	static char str[20];
		sprintf(str,"aa");
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)str,strlen(str));
		//control_mid();
		key[0].key_flag=0;
		HAL_TIM_Base_Start_IT(&htim6);
	}
	
	else if(key[0].key_longflag==1)
	{	
		key[0].key_longflag=0;
		page=2;	
	}
	
	
	 else if(key[1].key_flag==1)
	{	
		key[1].key_flag=0;
		AB_flag=1;
		X_target=cal_mid(point_data.A1[0],point_data.A2[0]);
		Y_target=AB_line[X_target];
		//control_A_B();
		mode++;
		if(mode>6)
		{mode=1;}
	}
	
		else if(key[2].key_flag==1)
	{	
		char debug_str[50];
		key[2].key_flag=0;
		control_mid();
		AB_flag=0;
		if(mode==1)
		{servo1_P+=0.1;}
		
		else if(mode==2)
		{servo1_I+=0.01;}
		
		else if(mode==3)
		{servo1_D+=0.1;}	
		
		else if(mode==4)
		{servo2_P+=0.1;}
		
		else if(mode==5)
		{servo2_I+=0.01;}	
		
		else if(mode==6)
		{servo2_D+=0.1;}	
		
	  pid_init(&servo1,POSITION_PID,servo1_P,servo1_I,servo1_D);	
	  pid_init(&servo2,POSITION_PID,servo2_P,servo2_I,servo2_D);	
      
      // 打印当前PID参数（调试用）
      sprintf(debug_str, "PID(+): %.2f,%.2f,%.2f,%.2f\r\n", 
              servo1_P, servo1_I, servo1_D, servo2_P);
      HAL_UART_Transmit(&huart1, (uint8_t *)debug_str, strlen(debug_str), 100);
      
      // 更新舵机PID参数后保存到Flash
      saveThePidParameter();	
	}
	
	else if(key[3].key_flag==1)
	{	
		key[3].key_flag=0;
		//control_A();
		AB_flag=0;
		if(mode==1)
		{servo1_P-=0.1;}
		
		else if(mode==2)
		{servo1_I-=0.01;}
		
		else if(mode==3)
		{servo1_D-=0.1;}	
		
		else if(mode==4)
		{servo2_P-=0.1;}
		
		else if(mode==5)
		{servo2_I-=0.01;}	
		
		else if(mode==6)
		{servo2_D-=0.1;}
	 pid_init(&servo1,POSITION_PID,servo1_P,servo1_I,servo1_D);	
	 pid_init(&servo2,POSITION_PID,servo2_P,servo2_I,servo2_D);
     
     // 更新舵机PID参数后保存到Flash
     saveThePidParameter();
		
	}	
}
	
	else if(page==2)
		
	{
		{if(key[0].key_flag==1)
	{	static char str[20];
		sprintf(str,"aa");
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)str,strlen(str));
		key[0].key_flag=0;
		
	}
	
	else if(key[0].key_longflag==1)
	{	
		key[0].key_longflag=0;
		page=1;	
	}
	
	
	 else if(key[1].key_flag==1)
	{	
		key[1].key_flag=0;
		
		AB_flag=1;
		X_target=cal_mid(point_data.A1[0],point_data.A2[0]);
		Y_target=AB_line[X_target];
		
//		mode++;
//		if(mode>=2)
//		{mode=1;}
	}
	
		else if(key[2].key_flag==1)
	{	
		key[2].key_flag=0;
		control_mid();
		AB_flag=0;
//		if(mode==1)
//		{servo2_P+=1;}
//		
//		else if(mode==2)
//		{servo2_D+=1;}
		
	}
	
	else if(key[3].key_flag==1)
	{	
		key[3].key_flag=0;
		control_A();
		AB_flag=0;
//		if(mode==1)
//		{servo2_P-=1;}
//		
//		else if(mode==2)
//		{servo2_D-=1;}	
	}
	}
	
	
}
	
}

