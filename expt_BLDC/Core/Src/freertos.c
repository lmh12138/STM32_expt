/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"

#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "BLDC.h"
#include "adc.h"
#include "stm32_2.8_lcd.h"
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t now_flag, last_flag;
bldc_t bldc;
uint32_t Adc_data;
uint32_t xxx1;
uint32_t Vbus_data;
uint32_t xxx2;
char str1[] = "ADC_data:";
char str2[] = "PWM:";
char str3[] = "Ref volcity:";
char str4[] = "Fdb volcity:";
char str5[] = "Vbus value:";
uint8_t button_flag = 0;
/* USER CODE END Variables */
/* Definitions for Task1 */
osThreadId_t Task1Handle;
const osThreadAttr_t Task1_attributes = {
    .name = "Task1",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
    .name = "myTask02",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
    .name = "myTask03",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for myTask04 */
osThreadId_t myTask04Handle;
const osThreadAttr_t myTask04_attributes = {
    .name = "myTask04",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for myTask05 */
osThreadId_t myTask05Handle;
const osThreadAttr_t myTask05_attributes = {
    .name = "myTask05",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for Timer_1ms */
osTimerId_t Timer_1msHandle;
const osTimerAttr_t Timer_1ms_attributes = {
    .name = "Timer_1ms"};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
GPIO_PinState KeyStateRead(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
/* USER CODE END FunctionPrototypes */

void StartTask1(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTask04(void *argument);
void StartTask05(void *argument);
void Timer_1ms_Callback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* Create the timer(s) */
    /* creation of Timer_1ms */
    Timer_1msHandle = osTimerNew(Timer_1ms_Callback, osTimerPeriodic, NULL, &Timer_1ms_attributes);

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    osTimerStart(Timer_1msHandle, 1);
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of Task1 */
    Task1Handle = osThreadNew(StartTask1, NULL, &Task1_attributes);

    /* creation of myTask02 */
    myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

    /* creation of myTask03 */
    myTask03Handle = osThreadNew(StartTask03, NULL, &myTask03_attributes);

    /* creation of myTask04 */
    myTask04Handle = osThreadNew(StartTask04, NULL, &myTask04_attributes);

    /* creation of myTask05 */
    myTask05Handle = osThreadNew(StartTask05, NULL, &myTask05_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartTask1 */
/**
 * @brief  Function implementing the Task1 thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask1 */
void StartTask1(void *argument) {
    /* USER CODE BEGIN StartTask1 */
    /* Infinite loop */
    for (;;) {
        osDelay(10);
    }
    /* USER CODE END StartTask1 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the myTask02 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument) {
    /* USER CODE BEGIN StartTask02 */
    /* Infinite loop */
    for (;;) {
        if (button_flag == 0 && bldc.BLDC_begin == BLDC_START) {
            BLDC_PhaseChange(&bldc);
            BLDCSend(bldc);
            button_flag = 1;
        }
        osDelay(4);
    }
    /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief Function implementing the myTask03 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument) {
    /* USER CODE BEGIN StartTask03 */
    /* Infinite loop */
    for (;;) {
        LCD_DisplayStringLine(Line0, (u8 *)str1);
        LCD_Draw_NUM(25, 300, bldc.adc_data);
        LCD_DisplayStringLine(Line2, (u8 *)str2);
        LCD_Draw_NUM(75, 300, bldc.pwm);
        LCD_DisplayStringLine(Line4, (u8 *)str3);
        LCD_Draw_NUM(125, 300, bldc.set_speed);
        LCD_DisplayStringLine(Line6, (u8 *)str4);
        LCD_Draw_NUM(175, 300, bldc.hall.volcity);
        LCD_DisplayStringLine(Line8, (u8 *)str5);
        LCD_Draw_NUM(225, 300, bldc.vbus_data);
        osDelay(1);
    }
    /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
 * @brief Function implementing the myTask04 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument) {
    /* USER CODE BEGIN StartTask04 */
    /* Infinite loop */
    for (;;) {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_Start_DMA(&hadc1, &Vbus_data, sizeof(Vbus_data));
        HAL_ADC_Start(&hadc3);
        HAL_ADC_Start_DMA(&hadc3, &Adc_data, sizeof(Adc_data));
        bldc.adc_data = (uint16_t)Adc_data;
        bldc.vbus_data = (uint16_t)Vbus_data / 7;
        // bldc.vbus_data = 24;
        bldc.pwm = bldc.adc_data * 100 / 4095;
        bldc.set_speed = bldc.pwm * BLDC_MAX_SPEED / 100;
        bldc.bldc_pid.ref = bldc.set_speed;
        osDelay(1);
    }
    /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
 * @brief Function implementing the myTask05 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask05 */
void StartTask05(void *argument) {
    /* USER CODE BEGIN StartTask05 */
    /* Infinite loop */
    for (;;) {
        if (KeyStateRead(GPIOG, GPIO_PIN_10) == GPIO_PIN_RESET) {
            last_flag = 1;
        }
        if ((KeyStateRead(GPIOG, GPIO_PIN_10) == GPIO_PIN_SET) && (last_flag == 1)) {
            bldc.BLDC_begin = 1 - bldc.BLDC_begin;
            last_flag = 0;
        }
        if (bldc.BLDC_begin == BLDC_STOP && button_flag == 1) {
            button_flag = 0;
        }
        osDelay(10);
    }
    /* USER CODE END StartTask05 */
}

/* Timer_1ms_Callback function */
void Timer_1ms_Callback(void *argument) {
    /* USER CODE BEGIN Timer_1ms_Callback */
    /* USER CODE END Timer_1ms_Callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
GPIO_PinState KeyStateRead(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET) {
        osDelay(10);
        if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET) {
            return GPIO_PIN_RESET;
        } else {
            return GPIO_PIN_SET;
        }
    }
    return GPIO_PIN_SET;
}
/* USER CODE END Application */
