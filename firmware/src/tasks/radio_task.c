#include "main.h"
#include "cmsis_os.h"
#include "message_types.h"
#include "radio.h"
#include "FreeRTOS.h"
#include "queue.h"

/* Reference the Global Queue */
extern osMessageQId AppQueueHandle;

/* --- Radio Callback Prototypes --- */
static void OnTxDone( void );
static void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
static void OnTxTimeout( void );
static void OnRxTimeout( void );
static void OnRxError( void );

/* Structure to hold pointers to our callback functions */
static RadioEvents_t RadioEvents;

/**
 * @brief  The Radio Task Entry Point
 */
void RadioTask_Entry(void const * argument)
{
    AppMsg_t msg;
    
    /* 1. Setup Radio Event Handlers */
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;

    /* 2. Initialize the Radio Hardware */
    Radio.Init( &RadioEvents );

    /* 3. Configure the Radio (Point-to-Point settings) */
    // Frequency: 915.0 MHz (Adjust for your region: 868.0 for EU)
    Radio.SetChannel( 915000000 );

    /* Setup TX Parameters
     * Power: 22 dBm (High Power for Wio-E5)
     * Bandwidth: 0 (125 kHz)
     * Datarate: 7 (Spreading Factor 7)
     * Coderate: 1 (4/5)
     * PreambleLen: 8
     */
    Radio.SetTxConfig( MODEM_LORA, 22, 0, 0, 7, 1, 8, false, true, 0, 0, false, 3000 );

    /* 4. The Infinite Loop */
    for(;;)
    {
        /* --- Wait for Data --- */
        /* portMAX_DELAY = Block forever until data arrives.
         * The CPU effectively sleeps here. 
         */
        if (xQueueReceive(AppQueueHandle, &msg, portMAX_DELAY) == pdPASS)
        {
            /* --- Data Received! Prepare Packet --- */
            uint8_t buffer[16];
            uint8_t len = 0;

            // Simple Byte Packing (Custom Protocol)
            buffer[len++] = msg.sensor_id; 
            
            // Encode Float Temperature as two bytes (e.g. 25.45 -> 25, 45)
            buffer[len++] = (uint8_t)msg.temperature; 
            buffer[len++] = (uint8_t)((msg.temperature - (int)msg.temperature) * 100);

            /* --- Send Packet --- */
            Radio.Send(buffer, len);
            
            /* Note: FreeRTOS continues running immediately. 
             * The 'OnTxDone' callback will fire when the hardware finishes.
             */
        }
    }
}

/* --- Callback Implementations --- */

static void OnTxDone( void )
{
    /* Transmission finished! Put Radio to Sleep to save battery */
    Radio.Sleep();
}

static void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    // Not used for Transmit-only node
}

static void OnTxTimeout( void ) { Radio.Sleep(); }
static void OnRxTimeout( void ) { Radio.Sleep(); }
static void OnRxError( void )   { Radio.Sleep(); }