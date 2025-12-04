#include "am2320.h"     // AM2320 sensor
#include "i2c_driver.h" // I2C driver
#include "lora_app.h"
#include "main.h"
#include "stm32wlxx_hal.h"
#include <stdio.h>
// #include "ads1110.h"         // Analog sensor
// #include "sensor_digital.h"  // Digital sensor
// #include "data_buffer.h"
#include "ads1110.h" // ADS1110 ADC
#include "board_config.h"
#include "gpio_driver.h"
#include "scheduler.h"
#include "uart_debug.h" // UART debug functions

void SystemClock_Config(void);
void LED_Init(void);
void LED_Blink(uint8_t times, uint32_t delay_ms);
void LED_On(void);
void LED_Off(void);

// static GPIO_Handle_t led = {
//     .port = LED_GPIO_PORT,
//     .pin = LED_GPIO_PIN,
//     .mode = GPIO_MODE_OUTPUT,
//     .pull = GPIO_PULL_NONE,
//     .speed = GPIO_SPEED_LOW
// };

int main(void) {
  HAL_Init();
  SystemClock_Config();

  // Initialize UART for debugging
  UART_Debug_Init();
  UART_Debug_Println("=== LoRa DAQ Module Starting ===");

  /* TODO: Initialize I2C bus */
  if (I2C_Init() != I2C_OK) {
    UART_Debug_Println("ERROR: I2C initialization failed!");
    while (1)
      ; // Stop here if I2C fails
  }
  UART_Debug_Println("I2C initialized");
  if (AM2320_Init() != I2C_OK) {
    UART_Debug_Println("WARNING: AM2320 not detected on I2C bus");
    UART_Debug_Println("Check connections: SDA=PA15, SCL=PB15");
    // Continue anyway for testing
  } else {
    UART_Debug_Println("AM2320 sensor detected!");
  }
  // ADS1110_Init();
  // ADS1110 configuration
  // ADS1110_Config_t adc_config = {
  //     .gain = ADS1110_GAIN_8,           // gain 8
  //     .data_rate = ADS1110_DR_60SPS,    // 60 samples/sec
  //     .mode = ADS1110_MODE_CONTINUOUS   // continuous mode
  // };

  // Digital_Sensor_Init();

  /* TODO: Initialize storage modules */
  // DataBuffer_Init();
  LoRa_App_Init();
  UART_Debug_Println("LoRa module initialized");

  Scheduler_Init();
  UART_Debug_Println("Scheduler initialized");

  Scheduler_SetSampleInterval(2);
  Scheduler_SetTxInterval(10);

  UART_Debug_Println("Sample interval: 2 sec, TX interval: 10 sec");
  UART_Debug_Println("System ready!");
  // UART_SendString("System started\r\n");

  // test lora data
  // uint8_t test_msg[] = "hello lora";

  // Variable for sensor data
  float temperature = 0.0f;
  float humidity = 0.0f;
  // float adc_voltage = 0.0f;
  uint8_t lora_buffer[64];
  uint16_t lora_len = 0;
  while (1) {
    if (Scheduler_ShouldSample()) {
      UART_Debug_Println("Sampling AM2320...");
      /*TODO: Read sensors */
      int8_t result = AM2320_Read(&temperature, &humidity);
      if (result == I2C_OK) {
        // Create debug message
        char debug_msg[64];
        snprintf(debug_msg, sizeof(debug_msg),
                 "Temp: %.1f C, Humidity: %.1f %%", temperature, humidity);
        UART_Debug_Println(debug_msg);

        // Prepare data for LoRa transmission
        lora_len = snprintf((char *)lora_buffer, sizeof(lora_buffer),
                            "T:%.1f,H:%.1f", temperature, humidity);
      } else if (result == I2C_TIMEOUT) {
        UART_Debug_Println("ERROR: AM2320 timeout (sensor not responding)");
      } else {
        UART_Debug_Println("ERROR: AM2320 I2C error");
      }
    }

    /* TODO: Read other sensors */

    //   int8_t result_adc = ADS1110_ReadVoltage(&adc_voltage);
    //   if (result_adc == I2C_OK) {
    //       char msg[64];
    //       snprintf(msg, sizeof(msg), "ADS1110: Voltage=%.4fV", adc_voltage);
    //       UART_Debug_Println(msg);
    //       ADS1110_ReadRaw(&adc_raw);
    //   } else {
    //       UART_Debug_Println("ADS1110: Read failed");
    //   }

    //   // Prepare data for LoRa transmission
    //   lora_len = snprintf((char *)lora_buffer, sizeof(lora_buffer),
    //                       "T:%.1f,H:%.1f,V:%.3f",
    //                       temperature, humidity, adc_voltage);
    // }

    if (Scheduler_ShouldTransmit()) {
      UART_Debug_Println("Transmitting...");
      // Send LoRa
      /* TODO: send actual data */
      // Send actual sensor data if available
      if (lora_len > 0) {
        LoRa_App_SendData(lora_buffer, lora_len);
        UART_Debug_Println("Sensor data transmitted.");
      } else {
        // Send test message if no sensor data
        uint8_t test_msg[] = "No sensor data";
        LoRa_App_SendData(test_msg, sizeof(test_msg));
        UART_Debug_Println("Test message transmitted.");
      }
      UART_Debug_Println("Transmission complete.");
    }

    __WFI(); // Wait for interrupt
  }
}

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType =
      RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3 | RCC_CLOCKTYPE_HCLK |
                                RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

void LED_Init(void) {
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef gpio = {0};
  gpio.Pin = GPIO_PIN_5;
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &gpio);
}

void LED_Blink(uint8_t times, uint32_t delay_ms) {
  for (uint8_t i = 0; i < times; i++) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    HAL_Delay(delay_ms);
  }
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

void LED_On(void) { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); }

void LED_Off(void) { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); }

void Error_Handler(void) {
  __disable_irq();
  while (1) {
    UART_Debug_Println("Error occurred! Halting.");
  }
}