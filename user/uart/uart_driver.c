#include "uart_driver.h"

// UART 初始化
void uart_init(void)
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
    
    USART_ClearFlag(USART1, USART_FLAG_TC);

    // 开启UART
    USART_Cmd(USART1, ENABLE);
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
char uart1_getc(void)
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

