#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>

// Initialize I2C peripheral
int8_t I2C_Init(void);

// Write to I2C device
int8_t I2C_Write(uint8_t dev_addr, uint8_t *data, uint16_t len);

// Read from I2C device
int8_t I2C_Read(uint8_t dev_addr, uint8_t *data, uint16_t len);

int8_t I2C_WriteRead(uint8_t dev_addr, uint8_t *tx_data, uint16_t tx_len, 
                     uint8_t *rx_data, uint16_t rx_len);

#endif