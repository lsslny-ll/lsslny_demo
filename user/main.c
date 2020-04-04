#include "stm32f10x.h"
#include "led_driver.h"
#include "sys_rcc.h"
#include "systick.h"
#include "uart_driver.h"

void bsp_init(void)
{
    RCC_DeInit();
    RCC_Configuration();
    systick_init(72);

    Led_InitConfig();
    uart_init();
}

int main(void)
{
    bsp_init();
    
    uart1_putc('H');
    uart1_putc('e');
    uart1_putc('l');
    uart1_putc('l');
    uart1_putc('o');
    uart1_putc(' ');
    
    uart1_puts("world\r\n", 7);
    
    while (uart1_getc() != 'a');
    
    while(1)
    {
        // Add your code here.
        Led_Ctrl(led1, led_on);
        //Led_Ctrl(led2, led_on);
        //Led_Ctrl(led3, led_on);
        delay_ms(1000);
        Led_Ctrl(led1, led_off);
        //Led_Ctrl(led2, led_off);
        //Led_Ctrl(led3, led_off);
        delay_ms(1000);
    }
}
