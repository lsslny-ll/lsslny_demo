#include "sys_rcc.h"

void RCC_Configuration(void)
{
    ErrorStatus HSE_Status;
    
    RCC_DeInit();                           // ��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
    RCC_HSEConfig(RCC_HSE_ON);              // ���ⲿ���پ���
    
    HSE_Status = RCC_WaitForHSEStartUp();   // �ȴ��ⲿ����ʱ��׼����
    if (SUCCESS == HSE_Status)
    {   
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   // ����FLASHԤ�����幦�ܣ�����FLASH��ȡ
        FLASH_SetLatency(FLASH_Latency_2);  // FLASH��������ʱ
        
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // ����AHB(HCLK)ʱ�ӵ���SYSCLK
        RCC_PCLK1Config(RCC_HCLK_Div2);     // ����APB1(PCLK1)ʱ�ӵ���AHB/2
        RCC_PCLK2Config(RCC_HCLK_Div1);     // ����APB2(PCLK2)ʱ�ӵ���AHBʱ��
        
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);    // ����PLLʱ��  (8M * 9)��Ƶ��72M
        RCC_PLLCmd(ENABLE);                 // ʹ��PLLʱ��
        
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);     // �ȴ�PLLʱ�Ӿ���
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);              // ����ϵͳʱ�� ����PLLʱ��
        while(RCC_GetSYSCLKSource() != 0x08);                   // ���PLLʱ���Ƿ���Ϊϵͳʱ��
    }
}

