#ifndef AM2320_H
#define AM2320_H

#include <stdint.h>

// AM2320 I2C address
#define AM2320_I2C_ADDR 0x5C

// Function codes
#define AM2320_FUNC_READ 0x03

// Register addresses
#define AM2320_REG_HUMIDITY_H 0x00
#define AM2320_REG_HUMIDITY_L 0x01
#define AM2320_REG_TEMP_H 0x02
#define AM2320_REG_TEMP_L 0x03

// Initialize AM2320 sensor
int8_t AM2320_Init(void);

// Read temperature and humidity
int8_t AM2320_Read(float *temperature, float *humidity);

// Read raw data (for debugging)
int8_t AM2320_ReadRaw(uint16_t *humidity_raw, uint16_t *temperature_raw);

#endif // AM2320_H