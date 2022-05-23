#include "BLDC.h"

#include "gpio.h"
#include "tim.h"
#include "usart.h"

extern uint8_t button_flag;

void BLDC_PhaseChange(bldc_t* bldc) {
    if (bldc->BLDC_begin == BLDC_START) {
        if (bldc->BLDC_direct == BLDC_POSITIVE) {
            if ((bldc->hall.hall_status.status_a == 1) &
                (bldc->hall.hall_status.status_b == 0) &
                (bldc->hall.hall_status.status_c == 1)) {
                bldc->BLDC_status = BLDC_AB;
            } else if ((bldc->hall.hall_status.status_a == 1) &
                       (bldc->hall.hall_status.status_b == 0) &
                       (bldc->hall.hall_status.status_c == 0)) {
                bldc->BLDC_status = BLDC_AC;
            } else if ((bldc->hall.hall_status.status_a == 1) &
                       (bldc->hall.hall_status.status_b == 1) &
                       (bldc->hall.hall_status.status_c == 0)) {
                bldc->BLDC_status = BLDC_BC;
            } else if ((bldc->hall.hall_status.status_a == 0) &
                       (bldc->hall.hall_status.status_b == 1) &
                       (bldc->hall.hall_status.status_c == 0)) {
                bldc->BLDC_status = BLDC_BA;
            } else if ((bldc->hall.hall_status.status_a == 0) &
                       (bldc->hall.hall_status.status_b == 1) &
                       (bldc->hall.hall_status.status_c == 1)) {
                bldc->BLDC_status = BLDC_CA;
            } else if ((bldc->hall.hall_status.status_a == 0) &
                       (bldc->hall.hall_status.status_b == 0) &
                       (bldc->hall.hall_status.status_c == 1)) {
                bldc->BLDC_status = BLDC_CB;
            }
        } else if (bldc->BLDC_direct == BLDC_NEGETIVE) {
            if ((bldc->hall.hall_status.status_a == 1) &
                (bldc->hall.hall_status.status_b == 0) &
                (bldc->hall.hall_status.status_c == 1)) {
                bldc->BLDC_status = BLDC_BA;
            } else if ((bldc->hall.hall_status.status_a == 1) &
                       (bldc->hall.hall_status.status_b == 0) &
                       (bldc->hall.hall_status.status_c == 0)) {
                bldc->BLDC_status = BLDC_CA;
            } else if ((bldc->hall.hall_status.status_a == 1) &
                       (bldc->hall.hall_status.status_b == 1) &
                       (bldc->hall.hall_status.status_c == 0)) {
                bldc->BLDC_status = BLDC_CB;
            } else if ((bldc->hall.hall_status.status_a == 0) &
                       (bldc->hall.hall_status.status_b == 1) &
                       (bldc->hall.hall_status.status_c == 0)) {
                bldc->BLDC_status = BLDC_AB;
            } else if ((bldc->hall.hall_status.status_a == 0) &
                       (bldc->hall.hall_status.status_b == 1) &
                       (bldc->hall.hall_status.status_c == 1)) {
                bldc->BLDC_status = BLDC_AC;
            } else if ((bldc->hall.hall_status.status_a == 0) &
                       (bldc->hall.hall_status.status_b == 0) &
                       (bldc->hall.hall_status.status_c == 1)) {
                bldc->BLDC_status = BLDC_BC;
            }
        }
    } else {
        bldc->BLDC_status = BLDC_00;
    }
}

void BLDCSend(bldc_t bldc) {
    TIM8->CCR1 = bldc.bldc_pid.output * 10000 / 100;
    TIM8->CCR2 = bldc.bldc_pid.output * 10000 / 100;
    TIM8->CCR3 = bldc.bldc_pid.output * 10000 / 100;
#ifdef PID
    TIM8->CCR1 = bldc.pwm * 10000 / 100;
    TIM8->CCR2 = bldc.pwm * 10000 / 100;
    TIM8->CCR3 = bldc.pwm * 10000 / 100;
#endif
    switch (bldc.BLDC_status) {
        case BLDC_AB:
            HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
            break;
        case BLDC_AC:
            HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
            break;
        case BLDC_BC:
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
            break;
        case BLDC_BA:
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
            break;
        case BLDC_CA:
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
            HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
            break;
        case BLDC_CB:
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
            HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
            break;
        case BLDC_00:
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
            break;
        default:
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
            break;
    }
}

void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM8) {
        HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
        HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
        __HAL_TIM_DISABLE_IT(&htim8, TIM_IT_BREAK);
    }
}
