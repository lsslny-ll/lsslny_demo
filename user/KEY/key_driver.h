#ifndef __KEY_DRIVER_H
#define __KEY_DRIVER_H

#include "stm32f10x.h"

enum key_type
{
    KEY1,
    KEY2
};

void key_init(void);
uint8_t key_scan(enum key_type type);

#endif
