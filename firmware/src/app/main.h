#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private defines -----------------------------------------------------------*/
#define RTC_N_PREDIV_S 10
#define RTC_PREDIV_S ((1 << RTC_N_PREDIV_S) - 1)
#define RTC_PREDIV_A ((1 << (15 - RTC_N_PREDIV_S)) - 1)

#include "board_config.h"
#include "stm32wlxx_hal.h"      // Pulls in the HAL drivers
#include "stm32wlxx_hal_conf.h" // Ensures HAL module selection
/* -------- Function Prototypes -------- */
void Error_Handler(void);
void SystemClock_Config(void);
void LED_Init(void);
void LED_Blink(uint8_t times, uint32_t delay_ms);
void LED_On(void);
void LED_Off(void);
uint32_t Get_Uptime_Seconds(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
