#include "eeprom.h"
#include <stdio.h>

// ����豸�Ƿ���Ӧ
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

// �����洢
// _ReadBuf: ��EEPROM�Ķ������ݵĻ�����
// _Address: ��EEPROM�Ķ����ݵ��׵�ַ
// Size: ��Ҫ�������ܹ��ֽڸ���
// 0 = ʧ�ܣ� 1 = �ɹ�
uint8_t eeprom_ReadBytes(uint8_t *_ReadBuf, uint16_t _Address, uint16_t Size)
{
    uint16_t i;
    
    // 1����ʼ
    i2c_start();
    // 2�����Ϳ�����
    i2c_send_byte(EE_DEV_ADDR | I2C_WR);
    // 3����ACK
    if (i2c_recv_ack() != 0)
    {
        goto cmd_fail;
    }
    // 4�����Ͷ���ַ
    i2c_send_byte(_Address);
    if (i2c_recv_ack() != 0)
    {
        goto cmd_fail;
    }
    
    // 5�����¿�ʼ
    i2c_start();
    // 6�����Ϳ�����
    i2c_send_byte(EE_DEV_ADDR | I2C_RD);
    if (i2c_recv_ack() != 0)
    {
        goto cmd_fail;
    }
    // 7��ѭ����ȡ����
    for (i = 0; i < Size; i++)
    {
        _ReadBuf[i] = i2c_recv_byte();
        // �����û���꣬����ACK��������꣬����NACK
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

// д������
// _WriteBuf: ��Ҫ��ŵ�EEPROM�Ļ�����
// _Address: ��Ҫд��EEPROM���׵�ַ
// Size: ��Ҫд����ܹ��ֽڸ���
// 0 = ʧ�ܣ� 1 = �ɹ�
uint8_t eeprom_WriteBytes(uint8_t *_WriteBuf, uint16_t _Address, uint16_t Size)
{
    uint16_t i,m;
    uint16_t Addr;
    
    Addr = _Address;
    for (i = 0; i < Size; i++)
    {
        // �����͵�1���ֽڻ�����Ҫ��ҳ����һҳ�ף���Ҫ���·��������źź͵�ַ
        if ((i == 0) || (Addr & (EE_PAGE_SIZE - 1)) == 0)
        {
            // ����STOP�������ڲ�д����
            i2c_stop();
            
            /*  ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ���ɣ�һ��С�� 10ms
                CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
            */
            for (m = 0; m < 100; m++)
            {
                // 1������I2C���������ź�
                i2c_start();
                // 2����������ֽ�
                i2c_send_byte(EE_DEV_ADDR | I2C_WR);
                // 3���ж������Ƿ��Ӧ
                if (i2c_recv_ack() == 0)
                {
                    break;
                }
            }
            // ��ʱû�м�⵽�豸��Ӧ
            if (m == 100)
            {
                // EEPROM ��ʱ
                goto cmd_fail;
            }
            // 4�������ֽڵ�ַ
            i2c_send_byte((uint8_t)Addr);
            // 5������ack
            if (i2c_recv_ack() != 0)
            {
                goto cmd_fail;  /* EEPROM������Ӧ�� */
            }
        }
        // 6�� ��ʼд������
        i2c_send_byte(_WriteBuf[i]);
        // 7������ack
        if (i2c_recv_ack() != 0)
        {
            goto cmd_fail;  /* EEPROM������Ӧ�� */
        }
        
        Addr++;
    }
    
    // ���ݳɹ����д�룬ֹͣI2Cռ������
    i2c_stop();
    return 1;
cmd_fail:
    i2c_stop();
    return 0;
}

// EEPROM ����
void eeprom_Erase(void)
{
    uint16_t i;
    uint8_t  buf[EE_SIZE];
    
    for (i = 0; i < EE_SIZE; i++)
    {
        buf[i] = 0xff;
    }
    
    // дEEPROM����ʼ��ַ��0�����ݳ����� EE_SIZE
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


