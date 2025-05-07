#ifndef __headfile_H
#define __headfile_H

#include "stm32f4xx_hal.h"

/* USER CODE BEGIN PV */
extern uint16_t servo_PWM;
extern _Bool trans_flag;
extern _Bool servo1_flag;
/*HAL生成*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/*标准库头文件*/
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
/*user_include*/
#include "KEY.h"
#include  "servo.h"
#include "flash.h"
#include "lcd.h"
#include "image.h"
#include "lcd_init.h"
#include "chuankou.h"
#include "uart.h"
#include "pid.h"
#include "ALL_INIT.h"
#endif
