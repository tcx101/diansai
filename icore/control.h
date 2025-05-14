#ifndef __control_H_
#define __control_H_ 
#include "headfile.h"
extern uint16_t X_target,Y_target,speed;
extern _Bool AB_flag;
void control_A();
void control_mid();
void control_A_B();
#endif
