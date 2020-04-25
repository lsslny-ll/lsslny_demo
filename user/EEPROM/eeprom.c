#include "eeprom.h"
#include <stdio.h>

// 检查设备是否响应
uint8_t eeprom_CheckOk(void)
{
    if (i2c_CheckDevice(EE_DEV_ADDR) == 0)
    {
        return 1;
    }
    else
    {
        i2c_stop();
        return 0;
    }
}

// 读出存储
// _ReadBuf: 从EEPROM的读出数据的缓存区
// _Address: 从EEPROM的读数据的首地址
// Size: 需要读出的总共字节个数
// 0 = 失败； 1 = 成功
uint8_t eeprom_ReadBytes(uint8_t *_ReadBuf, uint16_t _Address, uint16_t Size)
{
    uint16_t i;
    
    // 1、开始
    i2c_start();
    // 2、发送控制字
    i2c_send_byte(EE_DEV_ADDR | I2C_WR);
    // 3、读ACK
    if (i2c_recv_ack() != 0)
    {
        goto cmd_fail;
    }
    // 4、发送读地址
    i2c_send_byte(_Address);
    if (i2c_recv_ack() != 0)
    {
        goto cmd_fail;
    }
    
    // 5、重新开始
    i2c_start();
    // 6、发送控制字
    i2c_send_byte(EE_DEV_ADDR | I2C_RD);
    if (i2c_recv_ack() != 0)
    {
        goto cmd_fail;
    }
    // 7、循环读取数据
    for (i = 0; i < Size; i++)
    {
        _ReadBuf[i] = i2c_recv_byte();
        // 如果还没读完，则发送ACK，如果读完，则发送NACK
        if (i != Size - 1)
        {
            i2c_send_ack(0);
        }
        else
        {
            i2c_send_ack(1);
        }
    }
    
    i2c_stop();
    return 1;
cmd_fail:
    i2c_stop();
    return 0;
}

// 写入数据
// _WriteBuf: 需要存放到EEPROM的缓存区
// _Address: 需要写入EEPROM的首地址
// Size: 需要写入的总共字节个数
// 0 = 失败； 1 = 成功
uint8_t eeprom_WriteBytes(uint8_t *_WriteBuf, uint16_t _Address, uint16_t Size)
{
    uint16_t i,m;
    uint16_t Addr;
    
    Addr = _Address;
    for (i = 0; i < Size; i++)
    {
        // 当发送第1个字节或者需要翻页到下一页首，需要重新发起启动信号和地址
        if ((i == 0) || (Addr & (EE_PAGE_SIZE - 1)) == 0)
        {
            // 发送STOP，启动内部写操作
            i2c_stop();
            
            /*  通过检查器件应答的方式，判断内部写操作是否完成，一般小于 10ms
                CLK频率为200KHz时，查询次数为30次左右
            */
            for (m = 0; m < 100; m++)
            {
                // 1、发起I2C总线启动信号
                i2c_start();
                // 2、发起控制字节
                i2c_send_byte(EE_DEV_ADDR | I2C_WR);
                // 3、判断器件是否回应
                if (i2c_recv_ack() == 0)
                {
                    break;
                }
            }
            // 超时没有检测到设备回应
            if (m == 100)
            {
                // EEPROM 超时
                goto cmd_fail;
            }
            // 4、发送字节地址
            i2c_send_byte((uint8_t)Addr);
            // 5、发送ack
            if (i2c_recv_ack() != 0)
            {
                goto cmd_fail;  /* EEPROM器件无应答 */
            }
        }
        // 6、 开始写入数据
        i2c_send_byte(_WriteBuf[i]);
        // 7、发送ack
        if (i2c_recv_ack() != 0)
        {
            goto cmd_fail;  /* EEPROM器件无应答 */
        }
        
        Addr++;
    }
    
    // 数据成功完成写入，停止I2C占用总线
    i2c_stop();
    return 1;
cmd_fail:
    i2c_stop();
    return 0;
}

// EEPROM 擦除
void eeprom_Erase(void)
{
    uint16_t i;
    uint8_t  buf[EE_SIZE];
    
    for (i = 0; i < EE_SIZE; i++)
    {
        buf[i] = 0xff;
    }
    
    // 写EEPROM，起始地址是0，数据长度是 EE_SIZE
    if (eeprom_WriteBytes(buf, 0, EE_SIZE) == 0)
    {
        printf("Erase EEPROM ERROR!\r\n");
        return;
    }
    else
    {
        printf("Erase EEPROM SUCCESS!\r\n");
    }
}


