#include "lora_app.h"
#include "main.h"
#include "stm32wlxx_hal.h"
#include "uart_debug.h"

void SystemClock_Config(void);
void LED_Init(void);
void LED_Blink(uint8_t times, uint32_t delay_ms);
void LED_On(void);
void LED_Off(void);

int main(void) {
  // Initialize HAL
  HAL_Init();
  SystemClock_Config();
  LED_Init();
  // Initialize UART for debug output
  UART_Debug_Init();
  UART_Debug_Println("\r\n========================================");
  UART_Debug_Println("   LoRa Receiver Module Starting ");
  UART_Debug_Println("========================================");

  // Initialize LoRa in RX mode
  LoRa_App_Init_RX();
  UART_Debug_Println("Listening for packets...\r\n");
  LED_On();
  HAL_Delay(1000);
  LED_Off();
  // Main loop. wait for interrupts
  while (1) {
    __WFI();
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

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

void LED_Blink(uint8_t times, uint32_t delay_ms) {
  for (uint8_t i = 0; i < times; i++) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    HAL_Delay(delay_ms);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    HAL_Delay(delay_ms);
  }
}

void LED_On(void) { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); }

void LED_Off(void) { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); }

void Error_Handler(void) {
  __disable_irq();
  while (1) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    for (volatile int i = 0; i < 100000; i++)
      ;
  }
}