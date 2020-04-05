#include "stm32f10x.h"
#include "led_driver.h"
#include "sys_rcc.h"
#include "systick.h"
#include "uart_driver.h"
#include "key_driver.h"

void bsp_init(void)
{
    RCC_DeInit();
    RCC_Configuration();
    systick_init(72);

    Led_InitConfig();
    key_init();
    
    uart_init();
}

int main(void)
{
    int num = 93;
    
    bsp_init();
    
    PRINT("Well Come To Progream World!\r\n");
    PRINT("printf: num = %d!\r\n", num);
    
    while(1)
    {
        // Add your code here.
        if (key_scan(KEY1))
        {
            Led_Ctrl(led1, led_on);
            delay_ms(1000);
        }
        else
        {
            Led_Ctrl(led1, led_off);
        }
        if (key_scan(KEY2))
        {
            Led_Ctrl(led2, led_on);
            delay_ms(1000);
        }
        else
        {
            Led_Ctrl(led2, led_off);
        }
        //Led_Ctrl(led1, led_on);
        //Led_Ctrl(led2, led_on);
        //Led_Ctrl(led3, led_on);
        //delay_ms(1000);
        //Led_Ctrl(led1, led_off);
        //Led_Ctrl(led2, led_off);
        //Led_Ctrl(led3, led_off);
        //delay_ms(1000);
    }
}
