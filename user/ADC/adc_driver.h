#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

#include "stm32f10x.h"

void adc_init(void);
uint16_t Get_Adc(void);
float Get_AdcMath(void);

#endif

