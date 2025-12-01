#include "lora_app.h"
#include "radio.h"
#include "radio_board_if.h"
#include "stm32wlxx_hal.h"
#include "uart_debug.h"

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

void LoRa_App_Init(void)
{

    // Initialize Radio driver with callbacks
    Radio.Init(&RadioEvents);
    
    UART_Debug_Println("Setting radio channel...");
    Radio.SetChannel(915000000);  // 915 MHz for USA
    
    UART_Debug_Println("Configuring LoRa parameters...");
    
    // Configure LoRa parameters
    Radio.SetTxConfig(MODEM_LORA,      // LoRa mode
                      14,               // 14 dBm power
                      0,                // FSK deviation (N/A for LoRa)
                      0,                // Bandwidth: 0 = 125kHz
                      7,                // Datarate: SF7
                      1,                // Coderate: 4/5
                      8,                // Preamble length
                      false,            // Variable length
                      true,             // CRC on
                      false,            // No frequency hopping
                      0,                // Hop period (N/A)
                      false,            // IQ normal
                      3000);            // TX timeout 3s
    
    UART_Debug_Println("LoRa configuration complete!");
}


void LoRa_App_SendData(uint8_t *data, uint8_t size)
{
    Radio.Send(data, size);
}

// Radio event callback implementations
static void OnTxDone(void)
{
    // TODO: Handle transmission complete
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    // TODO: Handle received data
}

static void OnTxTimeout(void)
{
    // TODO: Handle TX timeout
}

static void OnRxTimeout(void)
{
    // TODO: Handle RX timeout
}

static void OnRxError(void)
{
    // TODO: Handle RX error
}