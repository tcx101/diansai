#include  "headfile.h"
uint8_t R_data[50];
char T_data[50];
//point point_data;
//_Bool found;

//uint16_t get_time(const char* entretime,const char* outtime)
//{
//	int year[2];
//	int mon[2];
//	int day[2];
//	int hour[2];
//	int min[2];
//	int sec[2];
//	uint64_t total_time_cnt;
//	uint16_t park_time;
//	sscanf(entretime,"%2d%2d%2d%2d%2d%2d",&year[0],&mon[0],&day[0],&hour[0],&min[0],&sec[0]);
//	
//	sscanf(outtime,"%2d%2d%2d%2d%2d%2d",&year[1],&mon[1],&day[1],&hour[1],&min[1],&sec[1]);
//	
//	total_time_cnt=(year[1]-year[0])*365*24*3600+(mon[1]-mon[0])*24*30*3600+(day[1]-day[0])*24*3600+(hour[1]-hour[0])*3600+(min[1]-min[0])*60+(sec[1]-sec[0]);
//	
//	if(total_time_cnt%3600)
//	{park_time=total_time_cnt/3600+1;}
//	else{park_time=total_time_cnt/3600;}
//	return park_time;
//		
//}

//void data_process()
//{
//	char T_data[50];
//	char type[5];
//	char ID[5];
//	
//	uint16_t park_time;
//	float park_cost;
//	uint16_t IDnum[8];
//	found=1;
//	if(strlen((char*)R_data)==24)
//	{
//		//sscanf(T_data,"%4[^:]:%4[^:]:%12s",type,ID,time);
//		//sscanf((char*)R_data,"%4[^:]:%4[^:]:%12s",type,ID,time);
//		sscanf((char*)R_data, "%4[^:]:%4[^:]:%12[^\r\n]", type, ID, time);
//		if(strcmp(type,"CNBR")==0)
//		{
//			uint8_t i;
//			for(i=0;i<=Data.total-1;i++)
//			{
//				/*离开车库的情况*/
//			  if(strcmp(ID,car[i].ID)==0)
//				{
//           /*计算停留时间*/
//					park_time=get_time(car[i].entertime,time);
//					park_cost=park_time*Para_CN.money;
//					found=0;
//					
//					Data.CNBR--;
//					if(Data.CNBR<=0)
//					{Data.CNBR=0;}
//					sprintf(T_data,"CNBR:%s:%d:%.2f",ID,park_time,park_cost);
//					HAL_UART_Transmit_DMA(&huart1,(uint8_t*)T_data,strlen(T_data));
//					break;
//				}	
//		  }
//			/*进入车库的情况*/
//			if(found)
//			{
//				uint8_t i,s;
//				for(i=0;i<=Data.total-1;i++)
//				{
//					if(car[i].ID[1]==0)
//					{
//						Data.CNBR++;
//						for(s=0;s<=3;s++)
//						{car[i].ID[s]=ID[s];}
//						for(s=0;s<=11;s++)
//						{car[i].entertime[s]=time[s];}
//						break;
//					}
//				}
//				  
//			}
//		}
//		else if(strcmp(type,"VNBR")==0)
//		{
//			uint8_t i;
//			for(i=0;i<=Data.total-1;i++)
//			{
//			  if(strcmp(ID,car[i].ID)==0)
//				{
//           /*计算停留时间*/
//					park_time=get_time(car[i].entertime,time);
//					park_cost=park_time*Para_VN.money;
//					found=0;
//					Data.VNBR--;
//					if(Data.VNBR<=0)
//					{Data.VNBR=0;}
//					sprintf(T_data,"VNBR:%s:%d:%.2f",ID,park_time,park_cost);
//					HAL_UART_Transmit_DMA(&huart1,(uint8_t*)T_data,strlen(T_data));
//					break;
//				}
//		  }
//			/*进入车库的情况*/
//			if(found)
//			{
//				uint8_t i,s;
//				for(i=0;i<=Data.total-1;i++)
//				{
//					if(car[i].ID[1]==0)
//					{
//						Data.VNBR++;
//						for(s=0;s<=3;s++)
//						{car[i].ID[s]=ID[s];}
//						for(s=0;s<=11;s++)
//						{car[i].entertime[s]=time[s];}
//						break;
//					}
//				}
//				  
//			}
//		}
//		else{sprintf(T_data,"Error1%s",&type[0]);
//	    HAL_UART_Transmit_DMA(&huart1,(uint8_t*)T_data,strlen(T_data));}
//		
//	}
//	else{
//		sprintf(T_data,"Error");
//	    HAL_UART_Transmit_DMA(&huart1,(uint8_t*)T_data,strlen(T_data));
//	//	HAL_UART_Transmit_DMA(&huart1,R_data,strlen((char*)R_data));
//	}
//}

//void Get_data()
//{
//	//sscanf((uint8_t*)R_data,"");
//	
//}






///*接收数据*/
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//	if(huart->Instance==USART1)
//	{
//		 /*数据处理*/
//        R_data[Size] = '\0'; // 手动添加终止符
//			// uart_process_UART();
//	}
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart2,R_data,sizeof(R_data));
//}	

void uart_R_start()
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,R_data,sizeof(R_data));
}

/*发送*/
void uart_T_yes()
{
	 
	sprintf(T_data,"yes\r\n");
	HAL_UART_Transmit_DMA(&huart2,(uint8_t*)T_data,strlen(T_data));
	
}

void uart_T_no()
{
	 
	sprintf(T_data,"no\r\n");
	HAL_UART_Transmit_DMA(&huart2,(uint8_t*)T_data,strlen(T_data));
	
}

void uart_R_test()
{
	
	HAL_UART_Transmit_DMA(&huart2,R_data,strlen((char*)R_data));
	
}




/*实例*/
//void uart_process_UART()
//{
//	
//	if(strcmp((char*)R_data,"TF1")==0)
//	{
//		
//		HAL_TIM_IC_Start_IT(&htim16,TIM_CHANNEL_1);
//	}
//	
//	else if(strcmp((char*)R_data,"TF2")==0)
//	{
//		
//		HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
//	}	
//	
//	else if(strcmp((char*)R_data,"PF1")==0)
//	{
//		
//		HAL_TIM_IC_Stop_IT(&htim16,TIM_CHANNEL_1);
//	}
//	
//	else if(strcmp((char*)R_data,"PF2")==0)
//	{
//		
//		HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1);
//	}		
//	
//	else if(strcmp((char*)R_data,"F1")==0)
//	{		
//	  sprintf(T_data,"F1:%dHZ",IC_R39);
//	  HAL_UART_Transmit_DMA(&huart1,(uint8_t*)T_data,strlen(T_data));		
//	}		
//	
//	else if(strcmp((char*)R_data,"F2")==0)
//	{		
//	  sprintf(T_data,"F2:%dHZ",IC_R40);
//	  HAL_UART_Transmit_DMA(&huart1,(uint8_t*)T_data,strlen(T_data));		
//	}	
//	
//  else
// {
//	 sprintf(T_data,"NULL");
//	 HAL_UART_Transmit_DMA(&huart1,(uint8_t*)T_data,strlen(T_data));	
//	
// }	
//	
//}

/*
一些常用的函数的使用方法

**从字符串中格式化读取数据
int sscanf(const char *str, const char *format, ...);

sscanf((char*)R_data, "%4[^:]:%4[^:]:%12[^\0]", type, ID, time);//意思是读取字符串R_data[],内容直到读取到'：'

注意这里%后的数字可以适当大一些（为了防止数组越界）

**比较字符串中的内容（相等）
 R_data[Size] = '\0'; // 接收的字符串需要手动添加终止符关键部分
int strcmp(const char *str1, const char *str2);
strcmp(type,"CNBR");

*/
