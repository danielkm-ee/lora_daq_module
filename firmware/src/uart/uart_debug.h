#ifndef UART_DEBUG_H
#define UART_DEBUG_H

#include <stdint.h>

void UART_Debug_Init(void);

void UART_Debug_Print(const char *str);

void UART_Debug_Println(const char *str);

#endif // UART_DEBUG_H