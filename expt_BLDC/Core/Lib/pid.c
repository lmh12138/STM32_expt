#include "pid.h"

#include "math.h"
#include "string.h"

/**
 * @brief 符号函数
 * @param input 输入
 * @retval 输入的符号，正负1或0
 */
float_t fsgn(float input) {
    return (input != 0.0f ? (input < 0.0f ? -1.0f : 1.0f) : 0.0f);
}

/**
 * @brief PID计算函数，位置式和增量式合在一起
 * @param PID结构体
 * @retval None
 */
void PID_Calc(struct PID_t *pid) {
    pid->error[2] = pid->error[1];        //上上次误差
    pid->error[1] = pid->error[0];        //上次误差
    pid->error[0] = pid->ref - pid->fdb;  //本次误差

    if (pid->PID_Mode == PID_POSITION)  //位置式PID
    {
        pid->error_sum += pid->error[0];  //积分上限判断
        if (pid->error_sum > pid->error_max)
            pid->error_sum = pid->error_max;
        if (pid->error_sum < -pid->error_max)
            pid->error_sum = -pid->error_max;

        pid->output = pid->KP * pid->error[0] + pid->KI * pid->error_sum + pid->KD * (pid->error[0] - pid->error[1]);
    }

    else if (pid->PID_Mode == PID_DELTA)  //增量式PID
    {
        pid->output += pid->KP * (pid->error[0] - pid->error[1]) + pid->KI * (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]) + pid->KI * pid->error[0];
    }

    else if (pid->PID_Mode == PID_COMP_POSITION) {
        pid->error_delta = pid->error[1] - pid->error[0];
        if (pid->error[0] > pid->range_rough)  // bangbang
        {
            pid->output = pid->outputMax;
            pid->error_sum = 0;
        } else if (pid->error[0] < -pid->range_rough)  // bangbang
        {
            pid->output = -pid->outputMax;
            pid->error_sum = 0;
        } else if (fabsf(pid->error[0]) < pid->range_fine)  //细调
        {
            pid->error_sum += pid->error[0];  //积分上限判断
            if (pid->error_sum > pid->error_max)
                pid->error_sum = pid->error_max;
            if (pid->error_sum < -pid->error_max)
                pid->error_sum = -pid->error_max;
            pid->output = pid->KP_fine * pid->error[1] + pid->KI * pid->error_sum + pid->KD * pid->error_delta;
        } else  //粗调
        {
            pid->output = pid->KP * (pid->error[1] + fsgn(pid->error[1]) * pid->compensation) + pid->KD * pid->error_delta;
            pid->error_sum = 0;
        }
    }

    /* 输出上限 */
    if (pid->output > pid->outputMax)
        pid->output = pid->outputMax;
    if (pid->output < -pid->outputMax)
        pid->output = -pid->outputMax;
}

void PID_Init(struct PID_t *pid) {
    memset(pid, 0, sizeof(pid));
    pid->KP = 5;
    pid->KI = 0;
    pid->KD = 0;
    pid->error_max = 100 * pid->KI;
    pid->PID_Mode = PID_POSITION;
    pid->outputMax = 100;
}