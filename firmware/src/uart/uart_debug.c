#include "uart_debug.h"
#include "usart_if.h"
#include <string.h>

void UART_Debug_Init(void) { vcom_Init(NULL); }

void UART_Debug_Print(const char *str) {
  if (str == NULL)
    return;
  vcom_Trace((uint8_t *)str, strlen(str));
}

void UART_Debug_Println(const char *str) {
  UART_Debug_Print(str);
  UART_Debug_Print("\r\n");
}
