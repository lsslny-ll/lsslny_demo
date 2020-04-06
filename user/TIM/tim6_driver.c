#include "tim6_driver.h"

// 目标： 配置定时器6为每 2ms 计时中断一次

void tim6_config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    
    TIM_TimeBaseInitStruct.TIM_Prescaler        = 719; // 72000000 / (719 + 1) == 100000Hz == 0.01ms
    TIM_TimeBaseInitStruct.TIM_Period           = 199;  // 0.01ms * (199+1) = 2ms
    TIM_TimeBaseInitStruct.TIM_CounterMode      = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
    
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM6, ENABLE);
}

void tim6_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    NVIC_InitStructure.NVIC_IRQChannel      = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void tim6_init(void)
{
    tim6_config();
    tim6_nvic_config();
}

