#include "stm32f10x.h"
#include "led_driver.h"
#include "sys_rcc.h"
#include "systick.h"

void bsp_init(void)
{
    RCC_DeInit();
    RCC_Configuration();
    systick_init(72);

    Led_InitConfig();
}

int main(void)
{
    bsp_init();
    
    while(1)
    {
        // Add your code here.
        Led_Ctrl(led1, led_on);
        Led_Ctrl(led2, led_on);
        Led_Ctrl(led3, led_on);
        delay_ms(1000);
        Led_Ctrl(led1, led_off);
        Led_Ctrl(led2, led_off);
        Led_Ctrl(led3, led_off);
        delay_ms(1000);
    }
}
