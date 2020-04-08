#include "adc_driver.h"

#define ADC1_DR_ADDRESS     ((uint32_t)0x40012400+0x4c)     // (uint32_t)&ADC1->DR

__IO uint16_t ADC_ConvertedValue[50] = {0};
__IO uint16_t After_filter; //  存放求平均值之后的结果

void adc_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// ADC  --  PC0
void adc_config(void)
{
    
    DMA_InitTypeDef  DMA_InitStructure;
    ADC_InitTypeDef  ADC_InitStructure;
    
    // DMA配置
    DMA_DeInit(DMA1_Channel1);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr    = ADC1_DR_ADDRESS;      //  ADC地址
    DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)&ADC_ConvertedValue[0];
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize            = 50;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    // 外设地址固定
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;  // 数据宽度为半字
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;    // 循环传输模式
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    // ADC配置
    ADC_InitStructure.ADC_Mode          = ADC_Mode_Independent;         // 独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode  = DISABLE;                      // 扫描模式，扫描模式用于多通道采集
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 开启连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 不使用外部触发转换
    ADC_InitStructure.ADC_DataAlign     = ADC_DataAlign_Right;          // 右对齐
    ADC_InitStructure.ADC_NbrOfChannel  = 1;                            // 要转换的通道数目
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // 配置ADC时钟，为PCLK2的8分频
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    
    // 配置ADC1的通道10为239.5个采样周期，序列为1
    ADC_RegularChannelConfig(ADC1,ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
    
    ADC_DMACmd(ADC1, ENABLE);
    
    ADC_Cmd(ADC1, ENABLE);
    
    // 复位校准寄存器
    ADC_ResetCalibration(ADC1);
    // 等待校准寄存器复位完成
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    // ADC校准
    ADC_StartCalibration(ADC1);
    // 等待校准完成
    while(ADC_GetCalibrationStatus(ADC1));
    
    // 由于没有采用外部触发，所以使用软件触发ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void adc_init(void)
{
    adc_gpio_config();
    adc_config();
}
    
// 求平均值函数
void filter(void)
{
    int sum = 0;
    uint8_t count;
    
    for (count = 0; count < 50; count++)
    {
        sum += ADC_ConvertedValue[count];
    }
    After_filter = sum/50;
}

// 获取通道AD值
uint16_t Get_Adc(void)
{
    filter();
    return After_filter;
}

// 获取采集电压值
// 公式： V(sample) = V(REF) * Value/ (0x0FFF + 1)
float Get_AdcMath(void)
{
    float Value;
    
    Value = (float)After_filter * (3.3 / 4096);
    
    return Value;
}




