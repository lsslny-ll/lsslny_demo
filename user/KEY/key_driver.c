#include "key_driver.h"
#include "systick.h"
/* 
    KEY1    --  PA0     按下 高电平
    KEY2    --  PC13    按下 低电平
*/

// 按键初始化
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t key_scan(enum key_type type)
{
    uint8_t ret = 0;
    
    switch(type)
    {
        case KEY1:  // KEY1 按下的电平为高
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET)
            {
                delay_ms(20);   // 消抖
                if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET)
                {
                    // 等待按键释放
                    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET);
                    // 表示按键被按下
                    ret = 1;
                }
            }
            break;
        case KEY2:  // KEY2 按下的电平为低
            if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == RESET)
            {
                delay_ms(20);   // 消抖
                if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == RESET)
                {
                    while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == RESET);
                    ret = 1;
                }
            }
            break;
        default:
            break;
    }
    
    return ret;
}


