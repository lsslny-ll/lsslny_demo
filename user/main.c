#include "stm32f10x.h"
#include "led_driver.h"

void delay(void)
{
    int i;
    for (i = 0; i < 0xffffff; i++);
}

int main(void)
{
    Led_InitConfig();
    
    
    while(1)
    {
        // Add your code here.
        Led_Ctrl(led1, led_on);
        Led_Ctrl(led2, led_on);
        Led_Ctrl(led3, led_on);
        delay();
        Led_Ctrl(led1, led_off);
        Led_Ctrl(led2, led_off);
        Led_Ctrl(led3, led_off);
        delay();
    }
}
