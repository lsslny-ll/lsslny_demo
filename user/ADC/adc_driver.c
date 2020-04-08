#include "adc_driver.h"

#define ADC1_DR_ADDRESS     ((uint32_t)0x40012400+0x4c)     // (uint32_t)&ADC1->DR

__IO uint16_t ADC_ConvertedValue[50] = {0};
__IO uint16_t After_filter; //  �����ƽ��ֵ֮��Ľ��

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
    
    // DMA����
    DMA_DeInit(DMA1_Channel1);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr    = ADC1_DR_ADDRESS;      //  ADC��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)&ADC_ConvertedValue[0];
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize            = 50;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    // �����ַ�̶�
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;  // ���ݿ��Ϊ����
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;    // ѭ������ģʽ
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    // ADC����
    ADC_InitStructure.ADC_Mode          = ADC_Mode_Independent;         // ����ADCģʽ
    ADC_InitStructure.ADC_ScanConvMode  = DISABLE;                      // ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // ��������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // ��ʹ���ⲿ����ת��
    ADC_InitStructure.ADC_DataAlign     = ADC_DataAlign_Right;          // �Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel  = 1;                            // Ҫת����ͨ����Ŀ
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // ����ADCʱ�ӣ�ΪPCLK2��8��Ƶ
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    
    // ����ADC1��ͨ��10Ϊ239.5���������ڣ�����Ϊ1
    ADC_RegularChannelConfig(ADC1,ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
    
    ADC_DMACmd(ADC1, ENABLE);
    
    ADC_Cmd(ADC1, ENABLE);
    
    // ��λУ׼�Ĵ���
    ADC_ResetCalibration(ADC1);
    // �ȴ�У׼�Ĵ�����λ���
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    // ADCУ׼
    ADC_StartCalibration(ADC1);
    // �ȴ�У׼���
    while(ADC_GetCalibrationStatus(ADC1));
    
    // ����û�в����ⲿ����������ʹ���������ADCת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void adc_init(void)
{
    adc_gpio_config();
    adc_config();
}
    
// ��ƽ��ֵ����
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

// ��ȡͨ��ADֵ
uint16_t Get_Adc(void)
{
    filter();
    return After_filter;
}

// ��ȡ�ɼ���ѹֵ
// ��ʽ�� V(sample) = V(REF) * Value/ (0x0FFF + 1)
float Get_AdcMath(void)
{
    float Value;
    
    Value = (float)After_filter * (3.3 / 4096);
    
    return Value;
}




