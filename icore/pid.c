#include "pid.h"
#include <math.h>
pid_t servo1;
pid_t servo2;

float servo1_P,servo1_I,servo1_D,servo2_P,servo2_I,servo2_D;

/*PID初始化部分*/
void pid_init(pid_t *pid, uint32_t mode, float p, float i, float d)//用于定义pid名字，pid模式，PID值

{
	pid->pid_mode = mode;
	pid->p = p;
	pid->i = i;
	pid->d = d;
} 

/*PID计算代码*/

void pid_cal(pid_t *pid)//需要pid的target和now，输出一个PID后的值
{
	// 计算当前偏差
	pid->error[0] = pid->target - pid->now;

	// 计算输出
	if(pid->pid_mode == DELTA_PID)  // 增量式
	{
		pid->pout = pid->p * (pid->error[0] - pid->error[1]);
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);
		pid->out += pid->pout + pid->iout + pid->dout;
	}
	else if(pid->pid_mode == POSITION_PID)  // 位置式
	{
		pid->pout = pid->p * pid->error[0];//err[0]是当前误差
		pid->iout += pid->i * pid->error[0];//积分累加
		pid->dout = pid->d * (pid->error[0] - pid->error[1]);//err[1]是上一状态的误差
		pid->out = pid->pout + pid->iout + pid->dout;
	}

	// 记录前两次偏差
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

}

void PID_servo1()
{
	servo1.target=point_data.mid[0];
	servo1.now=point_data.light[0];
  pid_cal(&servo1);
	set_servo(servo1_hadle,servo1.out);

}

void PID_servo2()
{
	servo2.target=point_data.mid[1];
	servo2.now=point_data.light[1];
  pid_cal(&servo2);
	set_servo(servo2_hadle,servo2.out);
}
//对pid参数进行flash写入
void saveThePidParameter (void) {
float pid_params[6] = {servo1.p, servo1.i, servo1.d, servo2.p, servo2.i, servo2.d};
flash_set_buffer_PID(pid_params, 6);
}
//加载pid参数
void getThePidParameter (void) {
	float pid_params[6];
 	flash_get_buffer_PID(pid_params, 6);
  if (isnan(pid_params[0]) || pid_params[0] < 0.01f) {
        // Flash中无有效数据，使用默认值
        return;
    }
	servo1.p = pid_params[0];
	servo1.i = pid_params[1];
	servo1.d = pid_params[2];
	servo2.p = pid_params[3];
	servo2.i = pid_params[4];
	servo2.d = pid_params[5];
pid_init(&servo1,POSITION_PID,servo1_P,servo1_I,servo1_D);
pid_init(&servo2,POSITION_PID,servo2_P,servo2_I,servo2_D);
}
    

