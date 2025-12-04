#ifndef ADS1110_H
#define ADS1110_H

#include <stdint.h>

#define ADS1110_I2C_ADDR 0x48 // Default to A0 variant

// Gain settings
typedef enum {
  ADS1110_GAIN_1 = 0x00, // ±2.048V
  ADS1110_GAIN_2 = 0x01, // ±1.024V
  ADS1110_GAIN_4 = 0x02, // ±0.512V
  ADS1110_GAIN_8 = 0x03  // ±0.256V
} ADS1110_Gain_t;

// Data rate settings
typedef enum {
  ADS1110_DR_240SPS = 0x00, // 240 SPS, 15-bit
  ADS1110_DR_60SPS = 0x04,  // 60 SPS, 16-bit
  ADS1110_DR_30SPS = 0x08,  // 30 SPS, 16-bit
  ADS1110_DR_15SPS = 0x0C   // 15 SPS, 16-bit
} ADS1110_DataRate_t;

// Conversion mode
typedef enum {
  ADS1110_MODE_CONTINUOUS = 0x00,
  ADS1110_MODE_SINGLE_SHOT = 0x10
} ADS1110_Mode_t;

// Configuration structure
typedef struct {
  ADS1110_Gain_t gain;
  ADS1110_DataRate_t data_rate;
  ADS1110_Mode_t mode;
} ADS1110_Config_t;

// Function prototypes
int8_t ADS1110_Init(ADS1110_Config_t *config);
int8_t ADS1110_SetConfig(ADS1110_Config_t *config);
int8_t ADS1110_ReadRaw(int16_t *raw_value);
int8_t ADS1110_ReadVoltage(float *voltage);

#endif // ADS1110_H