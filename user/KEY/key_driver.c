#include "key_driver.h"
#include "systick.h"
#include "led_driver.h"
#include "uart_driver.h"
/* 
    KEY1    --  PA0     ���� �ߵ�ƽ
    KEY2    --  PC13    ���� �͵�ƽ
*/

// ������ʼ��
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

enum key_type get_key_type(void)
{
    // KEY1 ���µĵ�ƽΪ��
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET)
    {
        delay_ms(20);   // ����
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET)
        {
            // �ȴ������ͷ�
            while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET);
            // ��ʾ����������
            return MSG_KEY1;
        }
    }
    // KEY2 ���µĵ�ƽΪ��
    if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == RESET)
    {
        delay_ms(20);   // ����
        if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == RESET)
        {
            while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == RESET);
            return MSG_KEY2;
        }
    }
    return MSG_NONE;
}

void key_scan(enum key_type type)
{
    switch(type)
    {
        case MSG_KEY1:  
            DBG_I("KEY1 DOWN!\r\n");
            Led_Ctrl(led1, led_on);
            break;
        case MSG_KEY2:  
            DBG_I("KEY2 DOWN!\r\n");
            Led_Ctrl(led1, led_off);
            break;
        default:
            break;
    }
}


