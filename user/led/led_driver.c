#include "led_driver.h"

/*
    LED1  --  PB0
    LED2  --  PC4
    LED3  --  PC3
*/

void Led_InitConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
    GPIO_SetBits(GPIOC, GPIO_Pin_3 | GPIO_Pin_4);
}

void Led_Ctrl(led_device device, led_status status)
{
    switch(device)
    {
        case led1:
            if (status == led_on)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_0);
            }
            else
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_0);
            }
            break;
        case led2:
            if (status == led_on)
            {
                GPIO_ResetBits(GPIOC, GPIO_Pin_4);
            }
            else
            {
                GPIO_SetBits(GPIOC, GPIO_Pin_4);
            }
            break;
        case led3:
            if (status == led_on)
            {
                GPIO_ResetBits(GPIOC, GPIO_Pin_3);
            }
            else
            {
                GPIO_SetBits(GPIOC, GPIO_Pin_3);
            }
            break;
        default:
            break;
    }
}


