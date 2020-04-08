#include "stm32f10x.h"
#include "led_driver.h"
#include "sys_rcc.h"
#include "systick.h"
#include "uart_driver.h"
#include "key_driver.h"
#include "tim6_driver.h"
#include "adc_driver.h"

void bsp_init(void)
{
    RCC_DeInit();
    RCC_Configuration();
    systick_init(72);
    tim6_init();

    Led_InitConfig();
    key_init();
    
    //uart1_config();
    uart_init_config();
    adc_init();
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
        delay_ms(1000);
        key_scan(get_key_type());
        USART_ReadData_String();
        delay_ms(1000);
        printf("ADC Channel10 Value£º %d  ,Vch10: %.3fV!!\r\n", Get_Adc(), Get_AdcMath());
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
