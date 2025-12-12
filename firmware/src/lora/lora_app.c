#include "lora_app.h"
#include "board_config.h"
#include "radio.h"
#include "radio_board_if.h"
#include "stm32wlxx_hal.h"
#include "uart_debug.h"
#include <stdio.h>

// Radio event callbacks (forward declarations)
static void OnTxDone(void);
static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
static void OnTxTimeout(void);
static void OnRxTimeout(void);
static void OnRxError(void);

// Radio events structure - THIS WAS MISSING!
static RadioEvents_t RadioEvents = {
    .TxDone = OnTxDone,
    .RxDone = OnRxDone,
    .TxTimeout = OnTxTimeout,
    .RxTimeout = OnRxTimeout,
    .RxError = OnRxError,
};

void LoRa_App_Init(void) {

  // Initialize Radio driver with callbacks
  Radio.Init(&RadioEvents);

  UART_Debug_Println("Setting radio channel (TX mode)...");
  Radio.SetChannel(LORA_FREQUENCY); // 915 MHz for USA

  UART_Debug_Println("Configuring LoRa parameters...");

  // Configure LoRa parameters
  Radio.SetTxConfig(MODEM_LORA,            // LoRa mode
                    LORA_TX_POWER_DBM,     // TX power in dBm
                    0,                     // FSK deviation (N/A for LoRa)
                    LORA_BANDWIDTH,        // Bandwidth: 0 = 125kHz
                    LORA_SPREADING_FACTOR, // Datarate: SF7
                    LORA_CODING_RATE,      // Coderate: 4/5
                    LORA_PREAMBLE_LENGTH,  // Preamble length
                    false,                 // Variable length
                    true,                  // CRC on
                    false,                 // No frequency hopping
                    0,                     // Hop period (N/A)
                    false,                 // IQ normal
                    3000);                 // TX timeout 3s

  UART_Debug_Println("LoRa configuration complete!");
}

void LoRa_App_Init_RX(void) {

  // Initialize Radio driver with callbacks
  Radio.Init(&RadioEvents);

  UART_Debug_Println("Setting radio channel (RX mode)...");
  Radio.SetChannel(LORA_FREQUENCY); // MATCH TX: 915 MHz
  UART_Debug_Println("Configuring LoRa RX parameters...");
  // Configure LoRa for RX; parameters match transmitter
  Radio.SetRxConfig(MODEM_LORA,            // LoRa mode
                    LORA_BANDWIDTH,        // Bandwidth: 0 = 125kHz
                    LORA_SPREADING_FACTOR, // Datarate: SF7
                    LORA_CODING_RATE,      // Coderate: 4/5
                    0,                    // bandwidthAfc: 0 (not used for LoRa)
                    LORA_PREAMBLE_LENGTH, // Preamble length
                    5,     // symbTimeout: 5 symbols (RX-specific)
                    false, // Variable length
                    0,     // payloadLen: 0 (not used with variable length)
                    true,  // CRC on
                    false, // No frequency hopping
                    0,     // Hop period
                    false, // IQ normal
                    true   // rxContinuous: true
  );

  UART_Debug_Println("Starting continuous receive mode...");

  Radio.Rx(0); // Start receiving (0 = continuous mode)

  UART_Debug_Println("Receiver ready!");
}

void LoRa_App_SendData(uint8_t *data, uint8_t size) { Radio.Send(data, size); }

// Radio event callback implementations
static void OnTxDone(void) {
  // TODO: Handle transmission complete
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi,
                     int8_t snr) {

  // Print packet info
  UART_Debug_Print("\r\n[Receiving Packet!] Size: ");
  char buf[32];

  // Print size
  snprintf(buf, sizeof(buf), "%u", size);
  UART_Debug_Print(buf);
  UART_Debug_Print(" bytes, RSSI: ");

  // Print RSSI
  snprintf(buf, sizeof(buf), "%d", rssi);
  UART_Debug_Print(buf);
  UART_Debug_Print(" dBm, SNR: ");

  // Print SNR
  snprintf(buf, sizeof(buf), "%d", snr);
  UART_Debug_Print(buf);
  UART_Debug_Println(" dB");

  // Print payload as hex
  UART_Debug_Print("Data (hex): ");
  for (uint16_t i = 0; i < size && i < 32; i++) {
    uint8_t byte = payload[i];
    char hex[4];
    hex[0] = "0123456789ABCDEF"[byte >> 4];
    hex[1] = "0123456789ABCDEF"[byte & 0x0F];
    hex[2] = ' ';
    hex[3] = '\0';
    UART_Debug_Print(hex);
  }
  UART_Debug_Println("");

  // Print payload as ASCII
  UART_Debug_Print("Data (ASCII): ");
  for (uint16_t i = 0; i < size; i++) {
    char c = payload[i];
    if (c >= 32 && c <= 126) {
      char str[2] = {c, '\0'};
      UART_Debug_Print(str);
    } else {
      UART_Debug_Print(".");
    }
  }
  UART_Debug_Println("\r\n");

  // CRITICAL: Restart receiving for next packet
  Radio.Rx(0);
}

static void OnTxTimeout(void) {
  // TODO: Handle TX timeout
}

static void OnRxTimeout(void) {
  // TODO: Handle RX timeout
}

static void OnRxError(void) {
  // TODO: Handle RX error
}