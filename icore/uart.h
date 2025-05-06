#ifndef __UART_H_
#define __UART_H_
#include "headfile.h"
void uart_R_start();
void uart_T_yes();
void uart_T_no();
void uart_R_test();
extern char T_data[50];
extern uint8_t R_data[50];

//typedef struct point
//{
//	uint16_t A1[2];
//	uint16_t A2[2];
//	uint16_t B1[2];
//	uint16_t B2[2];
//	uint16_t C1[2];	
//	uint16_t C2[2];
//	uint16_t D1[2];
//	uint16_t D2[2];
//	uint16_t light[2];
//	uint16_t mid[2];
//	
//}point;



#endif
