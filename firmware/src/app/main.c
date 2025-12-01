#include "stm32wlxx_hal.h"
#include "main.h"
#include "lora_app.h"
// #include "i2c_driver.h"      // Init I2C once
// #include "ads1110.h"         // Analog sensor
// #include "sensor_digital.h"  // Digital sensor
// #include "data_buffer.h"
#include "scheduler.h"
#include "uart_debug.h" // UART debug functions

void SystemClock_Config(void);
void LED_Init(void);
void LED_Blink(uint8_t times, uint32_t delay_ms);
void LED_On(void);
void LED_Off(void);

int main(void)
{  
    HAL_Init();
    SystemClock_Config();
    LED_Init();
    
    // Initialize UART for debugging
    UART_Debug_Init();
    UART_Debug_Println("=== LoRa DAQ Module Starting ===");

    /* TODO: Initialize I2C bus */
    // I2C_Init();
    
    /* TODO: Initialize sensors */
    // ADS1110_Init();
    // Digital_Sensor_Init();
    
    /* TODO: Initialize storage modules */
    // DataBuffer_Init();
    LoRa_App_Init();
    UART_Debug_Println("LoRa module initialized");

    Scheduler_Init();
    UART_Debug_Println("Scheduler initialized");
    
    Scheduler_SetSampleInterval(1);
    Scheduler_SetTxInterval(1);
    
    LED_Blink(10, 100);
    UART_Debug_Println("Sample interval: 1 sec, TX interval: 1 sec");
    UART_Debug_Println("System ready!");
    // UART_SendString("System started\r\n");

    // test lora data
    const uint8_t test_msg[] = "hello lora";
    
    while (1)
    {
        if (Scheduler_ShouldSample())
        {
            LED_On();
            UART_Debug_Println("Sampling...");
            /*TODO: Read sensors */
            LED_Off();
        }
        
        if (Scheduler_ShouldTransmit())
        {
            LED_Blink(3, 50);
            UART_Debug_Println("Transmitting...");
            // Send LoRa
            /* TODO: send actual data */
            LoRa_App_SendData(test_msg, sizeof(test_msg));  // Placeholder
            UART_Debug_Println("Transmission complete.");
        }
        
        __WFI();  // Wait for interrupt
    }
}
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK
                                |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                                |RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

void LED_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_5;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);
}

void LED_Blink(uint8_t times, uint32_t delay_ms)
{
    for(uint8_t i = 0; i < times; i++)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        HAL_Delay(delay_ms);
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

void LED_On(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}

void LED_Off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        UART_Debug_Println("Error occurred! Halting.");
    }
}