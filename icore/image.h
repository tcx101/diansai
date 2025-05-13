#ifndef __IMAGE_H
#define __IMAGE_H
#include "headfile.h"
#define MAX_X 480  // 最大X坐标（根据硬件调整）
#define MAX_Y 320  // 最大Y坐标（根据硬件调整）



void Fill_XLine(int X1, int Y1, int X2, int Y2, int* x_line);

void Fill_YLine(int X1, int Y1, int X2, int Y2, int* y_line);

int scale_480_to_128(int input);
	
int scale_320_to_64(int input);

uint16_t cal_mid(uint16_t point1,uint16_t point2);
/*横线*/
extern int AB_line[MAX_X];
extern int CD_line[MAX_X];
/*竖线*/
extern int BC_line[MAX_Y];
extern int AD_line[MAX_Y];
#endif
