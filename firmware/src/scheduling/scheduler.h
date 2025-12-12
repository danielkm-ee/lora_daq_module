#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

// Initialize timer for scheduling
void Scheduler_Init(void);

// Set how often to sample sensors (in seconds)
void Scheduler_SetSampleInterval(uint32_t seconds);

// Set how often to transmit (in seconds)
void Scheduler_SetTxInterval(uint32_t seconds);

// Check if it's time to sample (returns 1 and clears flag)
uint8_t Scheduler_ShouldSample(void);

// Check if it's time to transmit (returns 1 and clears flag)
uint8_t Scheduler_ShouldTransmit(void);

#endif