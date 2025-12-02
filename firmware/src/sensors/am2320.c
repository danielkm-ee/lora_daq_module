#include "am2320.h"
#include "i2c_driver.h"
#include "stm32wlxx_hal.h"

int8_t AM2320_Init(void) {
  uint8_t dummy;
  int8_t result;

  // wake up the sensor
  I2C_Write(AM2320_I2C_ADDR, NULL, 0);
  HAL_Delay(1);

  // read to verify sensor is present
  result = I2C_Read(AM2320_I2C_ADDR, &dummy, 1);

  if (result == I2C_TIMEOUT) {
    return I2C_ERROR; // Sensor not found
  }

  return I2C_OK;
}

int8_t AM2320_Read(float *temperature, float *humidity) {
  uint16_t humi_raw, temp_raw;
  int8_t result;

  // Read raw values
  result = AM2320_ReadRaw(&humi_raw, &temp_raw);
  if (result != I2C_OK) {
    return result;
  }

  // Convert to actual values
  *humidity = humi_raw / 10.0f; // Resolution: 0.1 %RH

  // Temperature can be negative (sign bit in MSB)
  if (temp_raw & 0x8000) {
    // Negative temperature
    temp_raw &= 0x7FFF;
    *temperature = -(temp_raw / 10.0f);
  } else {
    // Positive temperature
    *temperature = temp_raw / 10.0f;
  }

  return I2C_OK;
}

int8_t AM2320_ReadRaw(uint16_t *humidity_raw, uint16_t *temperature_raw) {
  uint8_t data[8];
  int8_t result;

  I2C_Write(AM2320_I2C_ADDR, NULL, 0);
  HAL_Delay(1); // Wait 800us minimum, we use 1ms to be safe

  // Send read command
  uint8_t cmd[3] = {
      AM2320_FUNC_READ,      // 0x03 - Read registers
      AM2320_REG_HUMIDITY_H, // 0x00 - Start at humidity high byte
      0x04                   // Read 4 bytes (humidity + temperature)
  };

  result = I2C_Write(AM2320_I2C_ADDR, cmd, 3);
  if (result != I2C_OK) {
    return result;
  }

  // Wait for sensor to prepare data
  HAL_Delay(2);

  result = I2C_Read(AM2320_I2C_ADDR, data, 8);
  if (result != I2C_OK) {
    return result;
  }

  if (data[0] != AM2320_FUNC_READ || data[1] != 0x04) {
    return I2C_ERROR; // Invalid response
  }

  *humidity_raw = (data[2] << 8) | data[3];
  *temperature_raw = (data[4] << 8) | data[5];

  return I2C_OK;
}