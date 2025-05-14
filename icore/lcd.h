#ifndef __LCD_H
#define __LCD_H		
#include "stm32f4xx_hal.h"
#include "headfile.h"

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);//指定区域填充颜色
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//在指定位置画一个点
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//在指定位置画一条线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//在指定位置画一个矩�?
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//在指定位置画一个圆

void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示汉字�?
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个32x32汉字

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示一个字�?
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示字符�?
uint32_t mypow(uint8_t m,uint8_t n);//求幂
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示整数变量
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示两位小数变量

void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);//显示图片

void LCD_show();
//画笔颜色
#define BLACK         	 0xFFFF
#define WHITE       	   0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define YELLOW           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define RED        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红�?
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝�?
#define LIGHTBLUE      	 0X7D7C	//浅蓝�?  
#define GRAYBLUE       	 0X5458 //灰蓝�?
#define LIGHTGREEN     	 0X841F //浅绿�?
#define LGRAY 			     0XC618 //浅灰�?(PANNEL),窗体背景�?
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜�?)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反�?)

#define X_1 1
#define X_2 9
#define X_3 17
#define X_4 25
#define X_5 33
#define X_6 41
#define X_7 49
#define X_8 57
#define X_9 65
#define X_10 73
#define X_11 81
#define X_12 89
#define X_13 97
#define X_14 105
#define X_15 113
#define X_16 121

#define Y_1 1
#define Y_2 17
#define Y_3 33
#define Y_4 49
#define Y_5 65
#define Y_6 81
#define Y_7 97
#define Y_8 113
#define Y_9 129
#define Y_10 144




#endif





