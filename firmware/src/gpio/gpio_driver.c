#include "gpio_driver.h"

void GPIO_Init(GPIO_Handle_t *gpio) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Enable clock for the GPIO port
  if (gpio->port == GPIOA) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
  } else if (gpio->port == GPIOB) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
  } else if (gpio->port == GPIOC) {
    __HAL_RCC_GPIOC_CLK_ENABLE();
  }

  // Configure GPIO
  GPIO_InitStruct.Pin = gpio->pin;
  GPIO_InitStruct.Mode = gpio->mode;
  GPIO_InitStruct.Pull = gpio->pull;
  GPIO_InitStruct.Speed = gpio->speed;

  HAL_GPIO_Init(gpio->port, &GPIO_InitStruct);
}

void GPIO_Write(GPIO_Handle_t *gpio, uint8_t state) {
  HAL_GPIO_WritePin(gpio->port, gpio->pin,
                    state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

uint8_t GPIO_Read(GPIO_Handle_t *gpio) {
  return HAL_GPIO_ReadPin(gpio->port, gpio->pin) == GPIO_PIN_SET ? 1 : 0;
}

void GPIO_Toggle(GPIO_Handle_t *gpio) {
  HAL_GPIO_TogglePin(gpio->port, gpio->pin);
}