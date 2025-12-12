#ifndef SENSOR_DIGITAL_H
#define SENSOR_DIGITAL_H

#include <stdint.h>

// Digital sensor on I2C bus

#define DIGITAL_SENSOR_ADDR 0x76 // I2C address?

int8_t Digital_Sensor_Init(void);
float Digital_Sensor_ReadTemp(void);
float Digital_Sensor_ReadHumidity(void);

#endif