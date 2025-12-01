#include "sensor_digital.h"
#include "i2c_driver.h"  // Uses same shared I2C driver!

int8_t Digital_Sensor_Init(void)
{
    // TODO: Configure digital sensor
    // Uses shared I2C driver
    return 0;
}

float Digital_Sensor_ReadTemp(void)
{
    return 25.0f;  // Placeholder
}

float Digital_Sensor_ReadHumidity(void)
{
    // TODO: Read humidity via I2C
    return 50.0f;  // Placeholder
}