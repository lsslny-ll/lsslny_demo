#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void systick_init(uint8_t SYSCLK);
void delay_us(uint16_t nus);
void delay_ms(uint16_t nms);
void delay_s(uint16_t ns);
#endif
