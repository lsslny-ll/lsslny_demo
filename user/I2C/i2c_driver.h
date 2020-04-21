#ifndef __I2C_DRIVER_H
#define __I2C_DRIVER_H

#include "stm32f10x.h"

/*
    Ó²¼þ½Ó¿Ú£º
    PB6     --      SCL1
    PB7     --      SDA1
*/
#define I2C_SCL_PIN     GPIO_Pin_6
#define I2C_SDA_PIN     GPIO_Pin_7
#define I2C_GPIO_PORT   GPIOB

void i2c_gpio_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_recv_ack(void);
void i2c_send_ack(uint8_t nack);
void i2c_send_byte(uint8_t ch);
uint8_t i2c_recv_byte(void);

#endif
