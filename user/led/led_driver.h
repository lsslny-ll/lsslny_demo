#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H

#include "stm32f10x.h"


typedef enum
{
    led1,
    led2,
    led3,
}led_device;

typedef enum
{
    led_on,
    led_off
}led_status;


void Led_InitConfig(void);
void Led_Ctrl(led_device device, led_status status);

#endif
