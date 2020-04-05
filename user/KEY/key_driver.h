#ifndef __KEY_DRIVER_H
#define __KEY_DRIVER_H

#include "stm32f10x.h"

enum key_type
{
    MSG_KEY1,
    MSG_KEY2,
    MSG_NONE = 0xFF
};

void key_init(void);
enum key_type get_key_type(void);
void key_scan(enum key_type type);

#endif
