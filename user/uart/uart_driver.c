#include "uart_driver.h"
#include <string.h>


int8_t Usart1Buff[USART1_BUFF] = {0};
USARTRecvData Usart1RecvStat = {
    0,
    &Usart1Buff[0],
    &Usart1Buff[0]
};

void clear_usart_buff(void)
{
    memset((int8_t*)Usart1Buff, (int8_t)0, USART1_BUFF);
    Usart1RecvStat.RecvCnt = 0;
    Usart1RecvStat.Recv_start = &Usart1Buff[0];
    Usart1RecvStat.Recv_end   = &Usart1Buff[0];
}

// UART 初始化
void uart1_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    // 1、开时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
    
    // 2、GPIO初始化
    // PA9 --> Tx 
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // PA10 --> Rx
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3、UART初始化
    USART_InitStructure.USART_BaudRate      = 115200;
    USART_InitStructure.USART_WordLength    = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits      = USART_StopBits_1;
    USART_InitStructure.USART_Parity        = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode          = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    USART_ClearFlag(USART1, USART_FLAG_TC);

    // 开启UART
    USART_Cmd(USART1, ENABLE);
}

void uart1_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    NVIC_InitStructure.NVIC_IRQChannel      = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd   = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
}

void uart_init_config(void)
{
    uart1_config();
    uart1_nvic_config();
    
}

// UART1发送一个字节数据
char uart1_putc(char ch)
{
    USART_SendData(USART1, ch);
    while(USART_GetFlagStatus(USART1 ,USART_FLAG_TC) != SET);
    return ch;
}
// UART1发送一个字符串
uint16_t uart1_puts(unsigned char* buff, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        uart1_putc(buff[i]);
    }
    return len;
}

// UART1接收一个字节数据
static char uart1_getc(void)
{
    char ch = 0;
    while(USART_GetFlagStatus(USART1 ,USART_FLAG_RXNE) != SET);
    ch = USART_ReceiveData(USART1);
    return ch;
}
// UART1接收一个字符串
uint16_t uart1_gets(unsigned char* buff, uint16_t len)
{
    uint16_t i = 0;
    for (i = 0; i < len; i++)
    {
        buff[i] = uart1_getc();
    }
    return len;
}

// printf 的重映射输出
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
    while(USART_GetFlagStatus(USART1 ,USART_FLAG_TC) == RESET);
    return (ch);
}

int fgetc(FILE *f)
{
    while(USART_GetFlagStatus(USART1 ,USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USART1);
}

extern uint8_t ReadUsartBack;
void USART_ReadData_String(void)
{
    if (ReadUsartBack)
    {
        ReadUsartBack = 0;
        printf("\r\n%s\r\n", Usart1RecvStat.Recv_start);
        printf("+++++\r\n");
        clear_usart_buff();
    }
}
