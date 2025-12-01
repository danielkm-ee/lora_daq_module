#include "scheduler.h"
#include "stm32wlxx_hal.h"
#include "main.h"
#include "stm32wlxx_it.h" // Interrupt handlers

TIM_HandleTypeDef htim2;

// Flags set by timer interrupt (volatile = can change in interrupt!)
static volatile uint8_t sample_flag = 0;
static volatile uint8_t tx_flag = 0;

// Intervals in seconds
static uint32_t sample_interval_sec = 1;    // Default: 1 second
static uint32_t tx_interval_sec = 60;       // Default: 60 seconds

// Counter incremented every second by timer interrupt
static volatile uint32_t second_counter = 0;

void Scheduler_Init(void)
{
    // Enable TIM2 clock
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    // Configure TIM2 structure
    htim2.Instance = TIM2;                      // Use TIM2 hardware
    htim2.Init.Prescaler = 48000 - 1;           // Subtract 1 because hardware operation adds 1 internally
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP; // Count up from 0
    htim2.Init.Period = 1000 - 1;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    // Initialize timer with HAL
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    
    // Start timer in interrupt mode
    if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    
    // Configure NVIC
    HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);  // Priority 5 (lower = higher priority)
    HAL_NVIC_EnableIRQ(TIM2_IRQn);          // Enable TIM2 interrupt
}

void Scheduler_SetSampleInterval(uint32_t seconds)
{
    sample_interval_sec = seconds;
}

void Scheduler_SetTxInterval(uint32_t seconds)
{
    tx_interval_sec = seconds;
}

uint8_t Scheduler_ShouldSample(void)
{
    if (sample_flag)
    {
        sample_flag = 0;  // Clear flag
        return 1;
    }
    return 0;
}

uint8_t Scheduler_ShouldTransmit(void)
{
    if (tx_flag)
    {
        tx_flag = 0;  // Clear flag
        return 1;
    }
    return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        second_counter++;  // Increment every second
        
        // Check if it's time to sample
        if (second_counter % sample_interval_sec == 0)
        {
            sample_flag = 1;
        }
        
        // Check if it's time to transmit
        if (second_counter % tx_interval_sec == 0)
        {
            tx_flag = 1;
        }
    }
}