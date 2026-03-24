#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t timeout;
    uint32_t limit;
    uint8_t  alive;
    uint8_t  fault;
} watchdog_t;


int8_t WDG_Init(watchdog_t *dev, uint32_t limit);

void WDG_Feed(watchdog_t *dev);
void WDG_Tick(watchdog_t *dev);
void WDG_Update(watchdog_t *dev);
bool WDG_IsFault(watchdog_t *dev);
bool WDG_IsAlive(watchdog_t *dev);
void WDG_ClearFault(watchdog_t *dev);
