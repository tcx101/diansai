#include "pid.h"
#include <math.h>
pid_t servo1;
pid_t servo2;

float servo1_P,servo1_I,servo1_D,servo2_P,servo2_I,servo2_D;

/*PID初始化函数*/
void pid_init(pid_t *pid, uint32_t mode, float p, float i, float d)//用于定义pid部分，pid模式，PID值

{
	pid->pid_mode = mode;
	pid->p = p;
	pid->i = i;
	pid->d = d;
} 

/*PID计算函数*/

void pid_cal(pid_t *pid)//需要pid的target和now，输出一个PID调节值
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
	servo1.target=X_target;
	servo1.now=point_data.light[0];
  pid_cal(&servo1);
	set_servo(servo1_hadle,servo1_mid-servo1.out);

}

void PID_servo2()
{
	servo2.target=Y_target;
	servo2.now=point_data.light[1];
  pid_cal(&servo2);
	set_servo(servo2_hadle,servo2_mid-servo2.out);
}

// 参数有效性检查函数，替代isnan
uint8_t is_valid_param(float value) {
    // 放宽检查条件，只要不是无穷大或NaN即可
    volatile uint32_t *v = (volatile uint32_t *)&value;
    uint32_t mask = 0x7F800000; // 指数部分全为1的掩码
    return (*v & mask) != mask; // 如果指数部分都是1，则可能是INF或NaN
}

/**
  * @brief 初始化PID参数
  * @param None
  * @retval None
  * @note 在系统启动时调用此函数，确保PID参数正确初始化
  */
void init_pid_parameters(void) {
    // 首先设置默认值
    servo1_P = 1.0f;   // 根据实际调试修改默认值
    servo1_I = 0.1f;
    servo1_D = 0.05f;
    servo2_P = 1.0f;
    servo2_I = 0.1f;
    servo2_D = 0.05f;
    
    // 使用默认值初始化PID控制器
    pid_init(&servo1, POSITION_PID, servo1_P, servo1_I, servo1_D);
    pid_init(&servo2, POSITION_PID, servo2_P, servo2_I, servo2_D);
    
    // 尝试从Flash加载之前保存的PID参数
    float pid_params[PID_TOTAL_PARAMS];
    HAL_StatusTypeDef status;
    
    // 从Flash读取参数
    status = flash_get_buffer_PID(pid_params, PID_TOTAL_PARAMS);
    
    if (status == HAL_OK) {
        // 简单验证参数范围（只要不是0或极大值就接受）
        if (pid_params[0] > 0.001f && pid_params[0] < 100.0f) {
            // 更新全局变量
            servo1_P = pid_params[0];
            servo1_I = pid_params[1];
            servo1_D = pid_params[2];
            servo2_P = pid_params[3];
            servo2_I = pid_params[4];
            servo2_D = pid_params[5];
            
            // 重新初始化PID控制器
            pid_init(&servo1, POSITION_PID, servo1_P, servo1_I, servo1_D);
            pid_init(&servo2, POSITION_PID, servo2_P, servo2_I, servo2_D);
        }
    }
}

// 将pid参数保存到flash中
HAL_StatusTypeDef saveThePidParameter(void) {
    // 更新全局变量
    servo1_P = servo1.p;
    servo1_I = servo1.i;
    servo1_D = servo1.d;
    servo2_P = servo2.p;
    servo2_I = servo2.i;
    servo2_D = servo2.d;
    
    // 创建参数数组
    float pid_params[PID_TOTAL_PARAMS];
    pid_params[0] = servo1_P;
    pid_params[1] = servo1_I;
    pid_params[2] = servo1_D;
    pid_params[3] = servo2_P;
    pid_params[4] = servo2_I;
    pid_params[5] = servo2_D;
    
    // 验证参数有效性
    for (uint8_t i = 0; i < PID_TOTAL_PARAMS; i++) {
        if (!is_valid_param(pid_params[i])) {
            // 参数无效，不保存
            return HAL_ERROR;
        }
    }
    
    // 保存到Flash
    HAL_StatusTypeDef status = flash_set_buffer_PID(pid_params, PID_TOTAL_PARAMS);
    return status;
}

// 读取pid参数
void getThePidParameter(void) {
    float pid_params[PID_TOTAL_PARAMS];
    HAL_StatusTypeDef status;
    
    status = flash_get_buffer_PID(pid_params, PID_TOTAL_PARAMS);
    
    // 检查读取状态和参数有效性
    if (status != HAL_OK) {
        // Flash读取失败，使用当前值
        return;
    }
    
    // 验证参数有效性
    for (uint8_t i = 0; i < PID_TOTAL_PARAMS; i++) {
        if (!is_valid_param(pid_params[i])) {
            // 参数无效，使用当前值
            return;
        }
    }
    
    // 更新全局变量
    servo1_P = pid_params[0];
    servo1_I = pid_params[1];
    servo1_D = pid_params[2];
    servo2_P = pid_params[3];
    servo2_I = pid_params[4];
    servo2_D = pid_params[5];
    
    // 使用更新后的全局变量初始化PID
    pid_init(&servo1, POSITION_PID, servo1_P, servo1_I, servo1_D);
    pid_init(&servo2, POSITION_PID, servo2_P, servo2_I, servo2_D);
}
    

