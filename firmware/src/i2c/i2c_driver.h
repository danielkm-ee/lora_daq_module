#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32wlxx_hal.h"
#include <stdint.h>

// Return codes
#define I2C_OK 0
#define I2C_ERROR -1
#define I2C_TIMEOUT -2
#define I2C_BUSY -3

// I2C timeout in milliseconds
#define I2C_TIMEOUT_MS 100

// Shared I2C driver

int8_t I2C_Init(void);
int8_t I2C_Write(uint8_t addr, uint8_t *data, uint16_t len);
int8_t I2C_Read(uint8_t addr, uint8_t *data, uint16_t len);
int8_t I2C_WriteRead(uint8_t addr, uint8_t *tx_data, uint16_t tx_len,
                     uint8_t *rx_data, uint16_t rx_len);

#endif