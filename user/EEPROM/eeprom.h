#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x.h"
#include "i2c_driver.h"

#define EE_DEV_ADDR     0xA0
#define EE_PAGE_SIZE    8
#define EE_SIZE         256

uint8_t eeprom_CheckOk(void);
uint8_t eeprom_ReadBytes(uint8_t *_ReadBuf, uint16_t _Address, uint16_t Size);
uint8_t eeprom_WriteBytes(uint8_t *_WriteBuf, uint16_t _Address, uint16_t Size);
void eeprom_Erase(void);

#endif
