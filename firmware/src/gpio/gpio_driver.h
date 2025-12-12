#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32wlxx_hal.h"
#include <stdint.h>

// GPIO configuration structure
typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
  uint32_t mode;
  uint32_t pull;
  uint32_t speed;
} GPIO_Handle_t;

// Pull-up/down options
#define GPIO_PULL_NONE GPIO_NOPULL
#define GPIO_PULL_UP GPIO_PULLUP
#define GPIO_PULL_DOWN GPIO_PULLDOWN

// GPIO speed options
#define GPIO_SPEED_LOW GPIO_SPEED_FREQ_LOW
#define GPIO_SPEED_MEDIUM GPIO_SPEED_FREQ_MEDIUM
#define GPIO_SPEED_HIGH GPIO_SPEED_FREQ_HIGH

// Function prototypes
void GPIO_Init(GPIO_Handle_t *gpio);
void GPIO_Write(GPIO_Handle_t *gpio, uint8_t state);
uint8_t GPIO_Read(GPIO_Handle_t *gpio);
void GPIO_Toggle(GPIO_Handle_t *gpio);

#endif // GPIO_DRIVER_H