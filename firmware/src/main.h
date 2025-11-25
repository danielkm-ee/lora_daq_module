/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define RTC_N_PREDIV_S 10
#define RTC_PREDIV_S ((1<<RTC_N_PREDIV_S)-1)
#define RTC_PREDIV_A ((1<<(15-RTC_N_PREDIV_S))-1)
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOB
#define PROB3_Pin GPIO_PIN_14
#define PROB3_GPIO_Port GPIOB
#define RF_CTRL3_Pin GPIO_PIN_3
#define RF_CTRL3_GPIO_Port GPIOC
#define BUT1_Pin GPIO_PIN_0
#define BUT1_GPIO_Port GPIOA
#define BUT1_EXTI_IRQn EXTI0_IRQn
#define PROB2_Pin GPIO_PIN_13
#define PROB2_GPIO_Port GPIOB
#define RF_CTRL2_Pin GPIO_PIN_5
#define RF_CTRL2_GPIO_Port GPIOC
#define PROB1_Pin GPIO_PIN_12
#define PROB1_GPIO_Port GPIOB
#define RF_CTRL1_Pin GPIO_PIN_4
#define RF_CTRL1_GPIO_Port GPIOC
#define BUT3_Pin GPIO_PIN_6
#define BUT3_GPIO_Port GPIOC
#define BUT2_Pin GPIO_PIN_1
#define BUT2_GPIO_Port GPIOA
#define BUT2_EXTI_IRQn EXTI1_IRQn
#define LED3_Pin GPIO_PIN_11
#define LED3_GPIO_Port GPIOB
#define USART1_RX_Pin GPIO_PIN_7
#define USART1_RX_GPIO_Port GPIOB
#define USART1_TX_Pin GPIO_PIN_6
#define USART1_TX_GPIO_Port GPIOB
#define USART2_RX_Pin GPIO_PIN_3
#define USART2_RX_GPIO_Port GPIOA
#define USART2_TX_Pin GPIO_PIN_2
#define USART2_TX_GPIO_Port GPIOA
#define PROB4_Pin GPIO_PIN_10
#define PROB4_GPIO_Port GPIOB
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void SystemClock_Config(void);

/* USER CODE BEGIN EFP */
void MX_GPIO_Init(void);
void MX_LPTIM1_Init(void);
/* Note: MX_SUBGHZ_Init is now in subghz.h, so we don't need it here */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* Define Wio-E5 specific pins here for easy reference */
#define LED_Pin             GPIO_PIN_5
#define LED_GPIO_Port       GPIOB

/* Setup for the SubGHz Radio SPI (Internal) */
/* These are standard for STM32WL, but good to have defined */
#define SUBGHZ_SPI_NSS_Pin  GPIO_PIN_4
#define SUBGHZ_SPI_NSS_Port GPIOA

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */