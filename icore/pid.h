#ifndef __PID_h_
#define __PID_h_
#include "headfile.h"

enum
{
  POSITION_PID,  // 位置式
  DELTA_PID,         // 增量式
};

typedef struct
{
	float target;	
	float now;
	float error[3];		
	float p,i,d;
	float pout, dout, iout;
	float out;   
	
	uint32_t pid_mode;

}pid_t;

extern pid_t servo1;
extern pid_t servo2;
extern float servo1_P,servo1_I,servo1_D,servo2_P,servo2_I,servo2_D;
void pid_cal(pid_t *pid);

void pid_init(pid_t *pid, uint32_t mode, float p, float i, float d);

void PID_servo1();

void PID_servo2();
void saveThePidParameter (void);
void getThePidParameter (void);
#endif
