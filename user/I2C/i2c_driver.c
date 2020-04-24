#include "i2c_driver.h"
#include "systick.h"

#if 0
    #define I2C_SCL_0()     GPIO_ResetBits(I2C_GPIO_PORT, I2C_SCL_PIN)
    #define I2C_SCL_1()     GPIO_SetBits(I2C_GPIO_PORT, I2C_SCL_PIN)
    #define I2C_SDA_0()     GPIO_ResetBits(I2C_GPIO_PORT, I2C_SDA_PIN)
    #define I2C_SDA_1()     GPIO_SetBits(I2C_GPIO_PORT,I2C_SDA_PIN)

    #define I2C_SDA_READ()  GPIO_ReadInputDataBit(I2C_GPIO_PORT, I2C_SDA_PIN)
#else
    #define I2C_SCL_0()     I2C_GPIO_PORT->BRR  = I2C_SCL_PIN
    #define I2C_SCL_1()     I2C_GPIO_PORT->BSRR = I2C_SCL_PIN
    #define I2C_SDA_0()     I2C_GPIO_PORT->BRR  = I2C_SDA_PIN
    #define I2C_SDA_1()     I2C_GPIO_PORT->BSRR = I2C_SDA_PIN

    #define I2C_SDA_READ()  ((I2C_GPIO_PORT->IDR & I2C_SDA_PIN) != 0)
#endif
void i2c_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // SCL SDA
    GPIO_InitStructure.GPIO_Pin     = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(I2C_GPIO_PORT, I2C_SCL_PIN | I2C_SDA_PIN);
}

static void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // SCL SDA
    GPIO_InitStructure.GPIO_Pin     = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);
}

static void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // SCL SDA
    GPIO_InitStructure.GPIO_Pin     = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);
}
// I2C start
void i2c_start(void)
{
    SDA_OUT();
    
    I2C_SDA_1();
    I2C_SCL_1();
    delay_us(8);
    I2C_SDA_0();
    delay_us(8);
    I2C_SCL_0();
}
// I2C stop
void i2c_stop(void)
{
    SDA_OUT();
    
    I2C_SDA_0();
    I2C_SCL_1();
    delay_us(8);
    I2C_SDA_1();
    delay_us(8);
}

// get a ack
uint8_t i2c_recv_ack(void)
{
    uint8_t ErrorTime = 0;
    SDA_IN();
    
    I2C_SDA_1();    // 置高是为了让从机写
    delay_us(4);
    I2C_SCL_1();    // 从机写完，主机拉高，主机读取
    delay_us(4);
    
    while(I2C_SDA_READ())
    {
        ErrorTime++;
        if (ErrorTime > 250)
        {
            // 超时，要么收到的是NACK，要么是设备坏了无应答，在这两种情况都需要stop iic
            i2c_stop();
            return 1;
        }
    }
    I2C_SCL_0();
    return 0;
}

// set a ack
void i2c_send_ack(uint8_t ack)
{
    SDA_OUT();
    
    I2C_SCL_0();
    delay_us(8);
    if (ack)
    {
        I2C_SDA_1();
    }
    else
    {
        I2C_SDA_0();
    }
    I2C_SCL_1();
    delay_us(8);    // 这里拉高是为了让从机读取
    I2C_SCL_0();
    delay_us(8);
}

// 发送一个字节数据
void i2c_send_byte(uint8_t ch)
{
    uint8_t i;
    
    SDA_OUT();
    
    I2C_SCL_0();
    for (i = 0; i < 8; i++)
    {
        if (ch & 0x80)
        {
            I2C_SDA_1();
        }
        else
        {
            I2C_SDA_0();
        }
        ch <<= 1;
        delay_us(5);
        I2C_SCL_1();
        delay_us(5);
        I2C_SCL_0();
        delay_us(5);
    }
}

// 接收一个字节数据
uint8_t i2c_recv_byte(void)
{
    uint8_t i;
    uint8_t ret = 0;
    
    SDA_IN();
    
    for (i = 0; i < 8; i++)
    {
        delay_us(5);
        I2C_SCL_1();
        delay_us(5);
        ret <<= 1;
        if (I2C_SDA_READ())
        {
            ret |= 0x01;
        }
        I2C_SCL_0();
    }
    return ret;
}

// 检测IIC总线设备
uint8_t i2c_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;
    
    i2c_start();
    
    // 发送设备地址+读写控制bit(0 = W, 1 = R)bit7 先传
    i2c_send_byte(_Address | I2C_WR);
    ucAck = i2c_recv_ack(); // 检测设备的ACK应答
    
    i2c_stop();
    
    return ucAck;
}



