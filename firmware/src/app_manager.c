/* src/app_manager.c */
#include "app_manager.h"
#include "message_types.h"

/* Import the "Work" functions from your task files */
/* (We haven't written these files yet, but the linker needs to know they exist) */
extern void SensorTask_Entry(void const * argument);
extern void RadioTask_Entry(void const * argument);

/* Define Global Handles */
osThreadId SensorTaskHandle;
osThreadId RadioTaskHandle;
osMessageQId AppQueueHandle;

void App_System_Init(void) {
    
    /* 1. Create the Message Queue
     * This holds 'AppMsg_t' structs defined in message_types.h
     */
    osMessageQDef(AppQueue, 10, AppMsg_t); 
    AppQueueHandle = osMessageCreate(osMessageQ(AppQueue), NULL);

    /* 2. Create the Sensor Task
     * Priority: Normal
     * Stack: 512 Words (Enough for I2C and math)
     */
    osThreadDef(SensTask, SensorTask_Entry, osPriorityNormal, 0, 512);
    SensorTaskHandle = osThreadCreate(osThread(SensTask), NULL);

    /* 3. Create the Radio Task
     * Priority: AboveNormal (Radio timing is critical)
     * Stack: 1024 Words (LoRaMac stack is heavy)
     */
    osThreadDef(RadTask, RadioTask_Entry, osPriorityAboveNormal, 0, 1024);
    RadioTaskHandle = osThreadCreate(osThread(RadTask), NULL);
}