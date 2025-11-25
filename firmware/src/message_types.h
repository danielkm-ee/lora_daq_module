#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#include <stdint.h>

// define what data should be look like

typedef struct {
    uint8_t sensor_id;
    float temperature;
    float humidity;
    uint32_t timestamp;
} AppMsg_t;

#endif /* MESSAGE_TYPES_H */