#include "stm32f10x.h"
#include "led_driver.h"
#include "sys_rcc.h"
#include "systick.h"
#include "uart_driver.h"
#include "key_driver.h"
#include "tim6_driver.h"
#include "adc_driver.h"
#include "i2c_driver.h"
#include "eeprom.h"

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
    
    i2c_gpio_init();
}

void eeprom_Test(void)
{
    uint16_t i;
    uint8_t  WriteBuf[EE_SIZE];
    uint8_t  ReadBuf[EE_SIZE];
    
    if (eeprom_CheckOk() == 0)
    {
        printf("Can not Check of EEPROM!\r\n");
        while(1); // 检测不到就停机
    }
    
    for (i = 0; i < EE_SIZE; i++)
    {
        WriteBuf[i] = i;
    }
    
    if (eeprom_WriteBytes(WriteBuf, 0, EE_SIZE) == 0)
    {
        printf("Write EEPROM ERROR!\r\n");
        return;
    }
    else
    {
        printf("Write EEPROM SUCCESS!\r\n");
    }
    
    delay_ms(1000);
    
    if (eeprom_ReadBytes(ReadBuf, 0, EE_SIZE) == 0)
    {
        printf("Read EEPROM ERROR!\r\n");
        return;
    }
    else
    {
        printf("Read EEPROM SUCCESS!\r\n");
    }
    
    for(i = 0; i < EE_SIZE; i++)
    {
        if (WriteBuf[i] != ReadBuf[i])
        {
            printf("0x%02X ", ReadBuf[i]);
            printf("Error: The read data is not equal to write data!\r\n");
            //return;
        }
        
        printf(" %02X", ReadBuf[i]);
        
        if ((i & 15) == 15)
		{
			printf("\r\n");	
		}	
    }
    //printf("EEPROM TEST SUCCESS!!!\r\n");
}

int main(void)
{
    int num = 93;
    
    bsp_init();
    
    PRINT("Well Come To Progream World!\r\n");
    PRINT("printf: num = %d!\r\n", num);
    
    PRINT("BEGIN EEPROM TEST-------\r\n");
    eeprom_Test();
    PRINT(" END  EEPROM TEST-------\r\n");
    
    while(1)
    {
        // Add your code here.
        //delay_ms(1000);
        //key_scan(get_key_type());
        //USART_ReadData_String();
        //delay_ms(1000);
        //printf("ADC Channel10 Value： %d  ,Vch10: %.3fV!!\r\n", Get_Adc(), Get_AdcMath());
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
