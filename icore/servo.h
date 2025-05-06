#ifndef __servo_H_
#define __servo_H_ 

#include "headfile.h"

#define servo1_hadle htim10
#define servo2_hadle htim11
#define servo3_hadle htim13
#define mid_value 1500
void set_servo(TIM_HandleTypeDef handle,uint16_t compare);
void servo_init(TIM_HandleTypeDef handle);

#endif
