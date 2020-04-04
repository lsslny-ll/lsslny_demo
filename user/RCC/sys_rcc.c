#include "sys_rcc.h"

void RCC_Configuration(void)
{
    ErrorStatus HSE_Status;
    
    RCC_DeInit();                           // 复位RCC外部设备寄存器到默认值
    RCC_HSEConfig(RCC_HSE_ON);              // 打开外部高速晶振
    
    HSE_Status = RCC_WaitForHSEStartUp();   // 等待外部高速时钟准备好
    if (SUCCESS == HSE_Status)
    {   
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   // 开启FLASH预读缓冲功能，加速FLASH读取
        FLASH_SetLatency(FLASH_Latency_2);  // FLASH操作的延时
        
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // 配置AHB(HCLK)时钟等于SYSCLK
        RCC_PCLK1Config(RCC_HCLK_Div2);     // 配置APB1(PCLK1)时钟等于AHB/2
        RCC_PCLK2Config(RCC_HCLK_Div1);     // 配置APB2(PCLK2)时钟等于AHB时钟
        
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);    // 配置PLL时钟  (8M * 9)倍频到72M
        RCC_PLLCmd(ENABLE);                 // 使能PLL时钟
        
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);     // 等待PLL时钟就绪
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);              // 配置系统时钟 等于PLL时钟
        while(RCC_GetSYSCLKSource() != 0x08);                   // 检查PLL时钟是否作为系统时钟
    }
}

