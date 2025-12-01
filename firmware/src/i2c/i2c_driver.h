#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>

// Shared I2C driver

int8_t I2C_Init(void);
int8_t I2C_Write(uint8_t addr, uint8_t *data, uint16_t len);
int8_t I2C_Read(uint8_t addr, uint8_t *data, uint16_t len);

#endif