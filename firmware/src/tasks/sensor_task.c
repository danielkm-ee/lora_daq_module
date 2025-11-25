#include "main.h"           // For HAL drivers (HAL_Delay, etc.)
#include "cmsis_os.h"       // For osDelay, osThreadDef
#include "message_types.h"  // Your "Contract" struct
#include "FreeRTOS.h"       // Required for native Queue functions
#include "queue.h"          // Required for xQueueSend

/* Reference the Global Queue created in app_manager.c */
extern osMessageQId AppQueueHandle;

/* * A helper to simulate sensor data 
 * (Remove this when you write your real I2C driver) 
 */
static float Read_Fake_Temperature(void) {
    static float temp = 20.0f;
    // Simulate temperature rising slightly
    temp += 0.1f; 
    if (temp > 35.0f) temp = 20.0f;
    return temp;
}

/**
 * @brief  The Sensor Task Entry Point
 * @param  argument: Not used
 * @retval None
 */
void SensorTask_Entry(void const * argument)
{
    /* 1. Initialization */
    // Initialize your I2C sensors here, e.g.:
    // HAL_I2C_Init(&hi2c1);
    
    AppMsg_t msg;
    msg.sensor_id = 0xA1; // Example ID for this device
    
    /* 2. The Infinite Loop */
    for(;;)
    {
        /* --- Step A: Acquire Data --- */
        // In the future: HAL_I2C_Master_Receive(...);
        msg.temperature = Read_Fake_Temperature();
        msg.humidity = 45.5f; // Constant dummy value
        msg.timestamp = HAL_GetTick(); // Current system time

        /* --- Step B: Send to Queue --- */
        /* * Logic: We verify the Queue exists, then push the data.
         * '10' is the wait time (ticks) if the queue is full.
         */
        if (AppQueueHandle != NULL) {
            // Note: We use xQueueSend (FreeRTOS native) because it handles 
            // copying the whole struct safely, unlike osMessagePut.
            if (xQueueSend(AppQueueHandle, &msg, 10) != pdPASS) {
                // Queue was full! Handle error (e.g., blink error LED)
            }
        }

        /* --- Step C: Sleep --- */
        // Sleep for 5 seconds (5000 ms)
        // This yields the CPU so the Radio Task can run.
        osDelay(5000);
    }
}