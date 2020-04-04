#include "systick.h"

static uint8_t  fac_us = 0;
static uint16_t fac_ms = 0;

// SYSTICK��ʱ�ӹ̶�ΪHCLK��ʱ�ӵ�1/8
// SYSCLK��ϵͳʱ��
void systick_init(uint8_t SYSCLK)
{
    SysTick->CTRL &= 0xfffffffb;    // bit2��գ�ʹ���ⲿʱ��   HCK/8
    fac_us = SYSCLK / 8;
    fac_ms = (uint16_t)fac_us * 1000;
}

// nus��ʱ����
void delay_us(uint16_t nus)
{
    uint32_t temp;
    
    SysTick->VAL  = 0;              // ��ռ�����
    SysTick->LOAD = nus * fac_us;   // ʱ�����
    SysTick->CTRL = 0x01;           // ��ʼ����
    do
    {
        temp = SysTick->CTRL;
    }while((temp & 0x01)&&(!(temp & (1 << 16))));   // �ȴ�ʱ�䵽��
    SysTick->CTRL = 0;              // �رռ�����
    SysTick->VAL  = 0;              // ��ռ�����
}

// ��ʱ nms
// ע��nms�ķ�Χ
// SysTick->LOAD Ϊ24λ�Ĵ��������ԣ������ʱΪ��
// nms <= 0xffffff * 8 * 1000 / SYSCLK
// SYSCLK��λΪHz��nms��λΪms
// ��72M������, nms <= 1864
void delay_ms(uint16_t nms)
{
    uint32_t temp;
    
    SysTick->VAL  = 0;              // ��ռ�����
    SysTick->LOAD = nms * fac_ms;   // ʱ�����
    SysTick->CTRL = 0x01;           // ��ʼ����
    do
    {
        temp = SysTick->CTRL;
    }while((temp & 0x01)&&(!(temp & (1 << 16))));   // �ȴ�ʱ�䵽��
    SysTick->CTRL = 0;              // �رռ�����
    SysTick->VAL  = 0;              // ��ռ�����
}

// ��ʱns
void delay_s(uint16_t ns)
{
    while (ns--)
    {
        delay_ms(1000);
    }
}
