#ifndef __CHUANKOU_H__
#define __CHUANKOU_H__
#include "usart.h"
#include <stdint.h>
#include <headfile.h>

extern char str[200];

typedef struct 
{
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
    uint16_t x3;
    uint16_t y3;
    uint16_t x4;
    uint16_t y4;
    uint16_t x5;
    uint16_t y5;
    uint16_t x6;
    uint16_t y6;
    uint16_t x7;
    uint16_t y7;
    uint16_t x8;
    uint16_t y8;
    uint16_t cx;  // 内部矩形中心点x坐标
    uint16_t cy;  // 内部矩形中心点y坐标
    uint16_t gx;  // 绿色激光点x坐标
    uint16_t gy;  // 绿色激光点y坐标
}zuobiao;

typedef struct point
{
	uint16_t A1[2];
	uint16_t A2[2];
	uint16_t B1[2];
	uint16_t B2[2];
	uint16_t C1[2];	
	uint16_t C2[2];
	uint16_t D1[2];
	uint16_t D2[2];
	uint16_t light[2];
	uint16_t mid[2];
	
}point;
extern point point_data;
extern zuobiao dingdian;

#endif
