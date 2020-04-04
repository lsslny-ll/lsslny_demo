#include "systick.h"

static uint8_t  fac_us = 0;
static uint16_t fac_ms = 0;

// SYSTICK的时钟固定为HCLK的时钟的1/8
// SYSCLK：系统时钟
void systick_init(uint8_t SYSCLK)
{
    SysTick->CTRL &= 0xfffffffb;    // bit2清空，使用外部时钟   HCK/8
    fac_us = SYSCLK / 8;
    fac_ms = (uint16_t)fac_us * 1000;
}

// nus延时函数
void delay_us(uint16_t nus)
{
    uint32_t temp;
    
    SysTick->VAL  = 0;              // 清空计数器
    SysTick->LOAD = nus * fac_us;   // 时间加载
    SysTick->CTRL = 0x01;           // 开始倒数
    do
    {
        temp = SysTick->CTRL;
    }while((temp & 0x01)&&(!(temp & (1 << 16))));   // 等待时间到达
    SysTick->CTRL = 0;              // 关闭计数器
    SysTick->VAL  = 0;              // 清空计数器
}

// 延时 nms
// 注意nms的范围
// SysTick->LOAD 为24位寄存器，所以，最大延时为：
// nms <= 0xffffff * 8 * 1000 / SYSCLK
// SYSCLK单位为Hz，nms单位为ms
// 对72M条件下, nms <= 1864
void delay_ms(uint16_t nms)
{
    uint32_t temp;
    
    SysTick->VAL  = 0;              // 清空计数器
    SysTick->LOAD = nms * fac_ms;   // 时间加载
    SysTick->CTRL = 0x01;           // 开始倒数
    do
    {
        temp = SysTick->CTRL;
    }while((temp & 0x01)&&(!(temp & (1 << 16))));   // 等待时间到达
    SysTick->CTRL = 0;              // 关闭计数器
    SysTick->VAL  = 0;              // 清空计数器
}

// 延时ns
void delay_s(uint16_t ns)
{
    while (ns--)
    {
        delay_ms(1000);
    }
}
