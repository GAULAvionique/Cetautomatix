#include "GAUL/Drivers/wdg.h"


int8_t WDG_Init(watchdog_t *dev, uint32_t limit) {
	if(!dev) return -1;

	dev->timeout = 0;
	dev->limit   = limit;
	dev->alive   = 0;
	dev->fault   = 0;

	return 0;
}


void WDG_Feed(watchdog_t *dev) {
	dev->timeout = 0;
	dev->alive   = 1;
	dev->fault   = 0;
}


void WDG_Tick(watchdog_t *dev) {
    if(dev->timeout < 0xFFFFFFFF) dev->timeout++;
}


void WDG_Update(watchdog_t *dev) {
    if(!dev->alive) return;

    if(dev->timeout > dev->limit) {
        dev->fault = 1;
    }
}


bool WDG_IsFault(watchdog_t *dev) {
    return (dev->fault != 0);
}


bool WDG_IsAlive(watchdog_t *dev) {
    return (dev->alive != 0);
}


void WDG_ClearFault(watchdog_t *dev) {
    dev->fault = 0;
}
