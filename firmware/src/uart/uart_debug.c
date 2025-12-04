#include "uart_debug.h"
#include "stm32wlxx_hal.h"
#include <string.h>

// UART handle
static UART_HandleTypeDef huart_debug;

void UART_Debug_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    
    // Configure TX pin (PA2)
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Configure RX pin (PA3)
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Configure USART2
    huart_debug.Instance = USART2;
    huart_debug.Init.BaudRate = 115200;
    huart_debug.Init.WordLength = UART_WORDLENGTH_8B;
    huart_debug.Init.StopBits = UART_STOPBITS_1;
    huart_debug.Init.Parity = UART_PARITY_NONE;
    huart_debug.Init.Mode = UART_MODE_TX_RX;
    huart_debug.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart_debug.Init.OverSampling = UART_OVERSAMPLING_16;
    
    HAL_UART_Init(&huart_debug);
}

void UART_Debug_Print(const char *str) {
    if (str == NULL)
        return;
    
    uint16_t len = strlen(str);
    HAL_UART_Transmit(&huart_debug, (uint8_t *)str, len, 1000);
}

void UART_Debug_Println(const char *str) {
    UART_Debug_Print(str);
    UART_Debug_Print("\r\n");
}