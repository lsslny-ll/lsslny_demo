#ifndef __UART_DRIVER_H
#define __UART_DRIVER_H

#include "stm32f10x.h"
#include <stdio.h>

#define DBG_INFO    "[INFO]"
#define DBG_WRING   "[WRING]"
#define DBG_ERROR   "[ERROR]"

#define UART1_DEBUG
#ifdef UART1_DEBUG
// TODO: ºóÐøÌí¼Ó£º __FILE__,__FUNCTION__,__LINE__
#define PRINT(...)              printf(__VA_ARGS__)
#else
#define PRINT(...)
#endif

void uart_init(void);
uint16_t uart1_puts(unsigned char* buff, uint16_t len);
uint16_t uart1_gets(unsigned char* buff, uint16_t len);

#endif
