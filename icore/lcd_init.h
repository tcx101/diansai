#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "stm32f4xx_hal.h"

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
// 1.8寸ST7735LCD的标准分辨率
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif


//-----------------LCD端口定义---------------- 

// SDA引脚连接到PD7(图片显示为67引脚)
#define LCD_MOSI_Clr() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET)//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET)

// SCL引脚连接到PD5(图片显示为69引脚)
#define LCD_SCLK_Clr() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET)//SCL=SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET)

// BLK引脚连接到PD1(图片显示为71引脚)
#define LCD_BLK_Clr()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET)

// RES引脚连接到PD3(图片第二张图显示为41引脚)
#define LCD_RES_Clr()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET)

// DC引脚连接到PD2(图片第二张图显示为45引脚)
#define LCD_DC_Clr()   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)
 		     
// CS引脚连接到PD4
#define LCD_CS_Clr()   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET)//CS
#define LCD_CS_Set()   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET)

// 延时函数声明
void delay_ms(uint32_t ms);

void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




