#ifndef __HALL_ENCODER_H__
#define __HALL_ENCODER_H__

#include <stdint.h>

typedef struct hall_status_t {
    uint8_t status_a;
    uint8_t status_b;
    uint8_t status_c;
} hall_status_t;

typedef struct hall_t {
    hall_status_t hall_status;
    uint16_t volcity;
} hall_t;

void ReadHallStatus(hall_status_t* hall_status);
void ReadVbusValue(uint16_t* vbus_data);

#endif
