// #include <stdint.h>
#include "i2c_driver.h"
// #include "stm32wlxx_hal.h"

// I2C handle for I2C2 peripheral
static I2C_HandleTypeDef hi2c2;

// Initialize I2C peripheral
int8_t I2C_Init(void) {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_I2C2_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Configure PA15 as I2C2_SDA
  GPIO_InitStruct.Pin = GPIO_PIN_15;

  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD; // AF: Alternate Function Open-Drain
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C2; // SDA function
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Configure PB10 as I2C2_SCL
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C2; // SCL function
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00702991; // 100kHz @ 48MHz
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
    return I2C_ERROR; // Initialization Error
  }
  // Configure analog filter
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
    return I2C_ERROR;
  }

  // Configure digital filter (0 = disabled)
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
    return I2C_ERROR;
  }

  return I2C_OK;
}

// Write to I2C device
int8_t I2C_Write(uint8_t dev_addr, uint8_t *data, uint16_t len) {
  HAL_StatusTypeDef status;

  // HAL expects 7-bit address shifted left by 1
  status =
      HAL_I2C_Master_Transmit(&hi2c2, dev_addr << 1, data, len, I2C_TIMEOUT_MS);

  if (status == HAL_OK)
    return I2C_OK;
  else if (status == HAL_TIMEOUT)
    return I2C_TIMEOUT;
  else if (status == HAL_BUSY)
    return I2C_BUSY;
  else
    return I2C_ERROR;
}

int8_t I2C_Read(uint8_t dev_addr, uint8_t *data, uint16_t len) {
  HAL_StatusTypeDef status;

  status =
      HAL_I2C_Master_Receive(&hi2c2, dev_addr << 1, data, len, I2C_TIMEOUT_MS);

  if (status == HAL_OK)
    return I2C_OK;
  else if (status == HAL_TIMEOUT)
    return I2C_TIMEOUT;
  else if (status == HAL_BUSY)
    return I2C_BUSY;
  else
    return I2C_ERROR;
}

int8_t I2C_WriteRead(uint8_t dev_addr, uint8_t *tx_data, uint16_t tx_len,
                     uint8_t *rx_data, uint16_t rx_len) {
  HAL_StatusTypeDef status;

  // First write the data (usually register address)
  status = HAL_I2C_Master_Transmit(&hi2c2, dev_addr << 1, tx_data, tx_len,
                                   I2C_TIMEOUT_MS);
  if (status != HAL_OK) {
    if (status == HAL_TIMEOUT)
      return I2C_TIMEOUT;
    if (status == HAL_BUSY)
      return I2C_BUSY;
    return I2C_ERROR;
  }

  // Then read the response
  status = HAL_I2C_Master_Receive(&hi2c2, dev_addr << 1, rx_data, rx_len,
                                  I2C_TIMEOUT_MS);

  if (status == HAL_OK)
    return I2C_OK;
  else if (status == HAL_TIMEOUT)
    return I2C_TIMEOUT;
  else if (status == HAL_BUSY)
    return I2C_BUSY;
  else
    return I2C_ERROR;
}