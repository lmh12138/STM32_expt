#include "hall_encoder.h"

#include "BLDC.h"
#include "gpio.h"
#include "pid.h"
#include "tim.h"
#include "usart.h"

uint32_t last_time, now_time;
uint32_t delta_time;
uint16_t last_volcity;
uint16_t test_volcity;
extern bldc_t bldc;

void ReadHallStatus(hall_status_t* hall_status) {
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_SET) {
        hall_status->status_a = 1;
    } else {
        hall_status->status_a = 0;
    }
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_SET) {
        hall_status->status_b = 1;
    } else {
        hall_status->status_b = 0;
    }
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET) {
        hall_status->status_c = 1;
    } else {
        hall_status->status_c = 0;
    }
}

void VolCalc() {
    delta_time = now_time - last_time;
    test_volcity = 41667 / delta_time;
    if (test_volcity > 3) {
        bldc.hall.volcity = 0.3 * last_volcity + 0.7 * test_volcity;
        last_volcity = test_volcity;
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    now_time = TIM3->CNT;
    if (now_time - last_time > 100) {
        VolCalc();
    }
    BLDC_PhaseChange(&bldc);
    bldc.bldc_pid.fdb = bldc.hall.volcity;
    PID_Calc(&bldc.bldc_pid);
    BLDCSend(bldc);
    ReadHallStatus(&bldc.hall.hall_status);
    last_time = TIM3->CNT;
    HAL_UART_Transmit(&huart1, (uint8_t*)&bldc.BLDC_status, sizeof(bldc.BLDC_status), 10);
}
