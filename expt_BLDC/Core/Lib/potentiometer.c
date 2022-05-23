#include "potentiometer.h"

#include "adc.h"

void ReadPotValue(uint16_t* adc_data) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_data, sizeof(adc_data));
}

void ReadVbusValue(uint16_t* vbus_data) {
    HAL_ADC_Start(&hadc3);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t*)vbus_data, sizeof(vbus_data));
}
