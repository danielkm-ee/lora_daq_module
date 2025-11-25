/* src/app_manager.h */
#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include "cmsis_os.h"

/* Global Task Handles (Exposed so other modules can reference them) */
extern osThreadId SensorTaskHandle;
extern osThreadId RadioTaskHandle;
extern osMessageQId AppQueueHandle;

/* Initialization Function */
void App_System_Init(void);

#endif /* APP_MANAGER_H */