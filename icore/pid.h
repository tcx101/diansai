#ifndef __PID_h_
#define __PID_h_
#include "headfile.h"

enum
{
  POSITION_PID,  // λ��ʽ
  DELTA_PID,         // ����ʽ
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

void pid_cal(pid_t *pid);

void pid_init(pid_t *pid, uint32_t mode, float p, float i, float d);



#endif
