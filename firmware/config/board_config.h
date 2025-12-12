#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm32wlxx_hal.h"

// GPIO Pin and Peripheral Configuration
// Below is for Wio-e5 board configuration
#define LED_GPIO_PORT GPIOB
#define LED_GPIO_PIN GPIO_PIN_5
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define BOARD_ID 1

#define I2C_TIMING 0x00702991 // 100kHz @ 48MHz

#define I2C_TIMEOUT_MS 100 // Timeout in milliseconds

#define I2C_SCL_GPIO_PORT GPIOB
#define I2C_SCL_GPIO_PIN GPIO_PIN_15
#define I2C_SCL_GPIO_AF GPIO_AF4_I2C2

#define I2C_SDA_GPIO_PORT GPIOA
#define I2C_SDA_GPIO_PIN GPIO_PIN_15
#define I2C_SDA_GPIO_AF GPIO_AF4_I2C2

#define USART_BAUDRATE 115200

#if defined(BOARD_CUSTOM)
// Custom PCB uses different UART pins
#define USARTx_TX_Pin GPIO_PIN_2
#define USARTx_TX_GPIO_Port GPIOA

#define USARTx_RX_Pin GPIO_PIN_3
#define USARTx_RX_GPIO_Port GPIOA
#else
// Wio-E5 (default)
#define USARTx_TX_Pin GPIO_PIN_6
#define USARTx_TX_GPIO_Port GPIOB

#define USARTx_RX_Pin GPIO_PIN_7
#define USARTx_RX_GPIO_Port GPIOB
#endif
// LoRa Configuration Parameters
#define LORA_FREQUENCY 915000000 // 915 MHz (US)

// TODO: Find in Radio.SetTxConfig call
#define LORA_TX_POWER_DBM 14    // TX power in dBm
#define LORA_BANDWIDTH 0        // 0=125kHz, 1=250kHz, 2=500kHz
#define LORA_SPREADING_FACTOR 7 // SF7..SF12
#define LORA_CODING_RATE 1      // 1=4/5, 2=4/6, 3=4/7, 4=4/8
#define LORA_PREAMBLE_LENGTH 8  // Preamble length in symbols
#define LORA_TX_TIMEOUT_MS 3000 // TX timeout

#endif // BOARD_CONFIG_H
