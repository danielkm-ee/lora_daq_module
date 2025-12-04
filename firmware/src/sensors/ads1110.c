#include "ads1110.h"
#include "i2c_driver.h"

// Voltage reference
#define ADS1110_VREF 2.048f

// Current configuration
static ADS1110_Config_t current_config;

// Initialize ADS1110
int8_t ADS1110_Init(ADS1110_Config_t *config) {
  // Store configuration
  current_config = *config;

  // Write configuration to device
  return ADS1110_SetConfig(config);
}

// Set configuration register
int8_t ADS1110_SetConfig(ADS1110_Config_t *config) {
  uint8_t config_byte;

  // Build configuration byte
  // Bit 7: ST/BSY = 1 (start conversion)
  // Bit 6-5: Reserved = 00
  // Bit 4: mode
  // Bit 3-2: data rate
  // Bit 1-0: gain
  config_byte = 0x80 | // Start conversion
                config->mode | config->data_rate | config->gain;

  // Write configuration to ADS1110
  return I2C_Write(ADS1110_I2C_ADDR, &config_byte, 1);
}

// Read raw 16-bit ADC value
int8_t ADS1110_ReadRaw(int16_t *raw_value) {
  uint8_t data[3];
  int8_t result;

  // Read 3 bytes: [MSB] [LSB] [Config]
  result = I2C_Read(ADS1110_I2C_ADDR, data, 3);

  if (result != I2C_OK) {
    return result;
  }

  // Combine MSB and LSB into 16-bit signed value
  *raw_value = (int16_t)((data[0] << 8) | data[1]);

  return I2C_OK;
}

// Read voltage at ADC input
int8_t ADS1110_ReadVoltage(float *voltage) {
  int16_t raw;
  int8_t result;
  float gain_divisor;

  // Read raw ADC value
  result = ADS1110_ReadRaw(&raw);
  if (result != I2C_OK) {
    return result;
  }

  // Get gain divisor based on configuration
  switch (current_config.gain) {
  case ADS1110_GAIN_1:
    gain_divisor = 1.0f;
    break;
  case ADS1110_GAIN_2:
    gain_divisor = 2.0f;
    break;
  case ADS1110_GAIN_4:
    gain_divisor = 4.0f;
    break;
  case ADS1110_GAIN_8:
    gain_divisor = 8.0f;
    break;
  default:
    gain_divisor = 1.0f;
  }

  // Convert to voltage
  // Formula: V = (raw / 32768) × (Vref / Gain)
  *voltage = ((float)raw / 32768.0f) * (ADS1110_VREF / gain_divisor);

  return I2C_OK;
}