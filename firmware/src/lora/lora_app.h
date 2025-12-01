#ifndef LORA_APP_H
#define LORA_APP_H

#include <stdint.h>

void LoRa_App_Init(void);
void LoRa_App_SendData(uint8_t *data, uint8_t size);

#endif