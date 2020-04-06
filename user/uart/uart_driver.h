#ifndef __UART_DRIVER_H
#define __UART_DRIVER_H

#include "stm32f10x.h"
#include <stdio.h>

#define UART1_DEBUG
#ifdef UART1_DEBUG
// TODO: 后续添加： __FILE__,__FUNCTION__,__LINE__
#define PRINT(...)              printf(__VA_ARGS__)
#define DBG_I(...)              printf("INFO:" __VA_ARGS__)
#define DBG_W(...)              printf("WRING:" __VA_ARGS__)
#define DBG_E(...)              printf("ERROR:" __VA_ARGS__)
#else
#define PRINT(...)
#define DBG_I(...)
#define DBG_W(...)
#define DBG_E(...)
#endif

#define USART1_BUFF     512

// 环形内存存储串口接收数据
typedef struct arg_USARTRecvData
{
    int32_t  RecvCnt;
    int8_t  *Recv_start;
    int8_t  *Recv_end;
}USARTRecvData;

extern int8_t Usart1Buff[USART1_BUFF];
extern USARTRecvData Usart1RecvStat;

void uart1_config(void);
void uart_init_config(void);
char uart1_putc(char ch);
uint16_t uart1_puts(unsigned char* buff, uint16_t len);
uint16_t uart1_gets(unsigned char* buff, uint16_t len);
void USART_ReadData_String(void);
#endif
