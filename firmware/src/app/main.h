#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus



extern "C" {
#endif

/* Private defines -----------------------------------------------------------*/
#define RTC_N_PREDIV_S 10
#define RTC_PREDIV_S ((1<<RTC_N_PREDIV_S)-1)
#define RTC_PREDIV_A ((1<<(15-RTC_N_PREDIV_S))-1)

#define USART_BAUDRATE 115200

#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOB

#define BUT1_Pin GPIO_PIN_13
#define BUT1_GPIO_Port GPIOB
#define BUT1_EXTI_IRQn EXTI15_10_IRQn

#define RF_CTRL1_Pin GPIO_PIN_4
#define RF_CTRL1_GPIO_Port GPIOA

#define RF_CTRL2_GPIO_Port GPIOA
#define RF_CTRL2_Pin GPIO_PIN_5

#define USARTx_RX_Pin GPIO_PIN_6
#define USARTx_RX_GPIO_Port GPIOB

#define USARTx_TX_Pin GPIO_PIN_7
#define USARTx_TX_GPIO_Port GPIOB

#include "stm32wlxx_hal.h"   // Pulls in the HAL drivers
#include "stm32wlxx_hal_conf.h"  // Ensures HAL module selection

/* -------- Pin Definitions -------- */
#define LED_PIN        GPIO_PIN_5
#define LED_PORT       GPIOB

/* -------- Function Prototypes -------- */
void Error_Handler(void);
void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
