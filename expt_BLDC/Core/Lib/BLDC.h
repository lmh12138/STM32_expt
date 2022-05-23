#ifndef __BLDC_H__
#define __BLDC_H__

#include "hall_encoder.h"
#include "pid.h"
#include "potentiometer.h"

#define BLDC_MAX_SPEED 67

typedef enum BLDC_direct_e {
    BLDC_POSITIVE = 0,
    BLDC_NEGETIVE
} BLDC_direct_e;

typedef enum BLDC_begin_e {
    BLDC_STOP = 0,
    BLDC_START
} BLDC_begin_e;

typedef enum BLDC_status_e {
    BLDC_00 = 0,
    BLDC_AB,
    BLDC_AC,
    BLDC_BC,
    BLDC_BA,
    BLDC_CA,
    BLDC_CB
} BLDC_status_e;

typedef struct bldc_t {
    BLDC_begin_e BLDC_begin;
    BLDC_direct_e BLDC_direct;
    BLDC_status_e BLDC_status;
    hall_t hall;
    uint16_t adc_data;
    uint16_t vbus_data;
    uint16_t pwm;
    uint16_t set_speed;
    struct PID_t bldc_pid;
} bldc_t;

void BLDC_PhaseChange(bldc_t* bldc);
void BLDCSend(bldc_t bldc);

#endif
