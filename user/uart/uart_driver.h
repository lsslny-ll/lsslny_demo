#ifndef __UART_DRIVER_H
#define __UART_DRIVER_H

#include "stm32f10x.h"

void uart_init(void);
char uart1_putc(char ch);
uint16_t uart1_puts(unsigned char* buff, uint16_t len);
char uart1_getc(void);
uint16_t uart1_gets(unsigned char* buff, uint16_t len);

#endif
