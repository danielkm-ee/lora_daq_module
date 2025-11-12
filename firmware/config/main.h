#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

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
