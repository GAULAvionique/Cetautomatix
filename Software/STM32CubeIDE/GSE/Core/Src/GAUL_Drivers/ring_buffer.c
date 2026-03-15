#include "GAUL_Drivers/ring_buffer.h"
#include <string.h>


int8_t RingBuffer_Init(ring_buffer_t *dev, void *buffer, uint16_t size, uint16_t item_size) {
	if(!dev || !buffer || size == 0) return -1;

    dev->buffer 	= buffer;
    dev->size 		= size;
    dev->item_size 	= item_size;
    dev->head 		= 0;
    dev->tail 		= 0;

    return 0;
}

uint16_t RingBuffer_Available(ring_buffer_t *dev) {
    if(dev->head >= dev->tail) return dev->head - dev->tail;

    return dev->size - dev->tail + dev->head;
}

uint16_t RingBuffer_Write(ring_buffer_t *dev, const void *data, uint16_t len) {
    uint16_t i;
    const uint8_t *p_data = (const uint8_t *)data;

    for(i = 0; i < len; i++) {
        uint16_t next = (dev->head + 1) % dev->size;
        if(next == dev->tail) break;

        void *dest = (uint8_t *)dev->buffer + (dev->head * dev->item_size);
        memcpy(dest, p_data + (i * dev->item_size), dev->item_size);

        dev->head = next;
    }

    return i;
}

uint16_t RingBuffer_Read(ring_buffer_t *dev, void *data, uint16_t len) {
    uint16_t i;

    for(i = 0; i < len; i++) {
        if(dev->tail == dev->head) break;

        void *src = (uint8_t *)dev->buffer + (dev->tail * dev->item_size);
        memcpy(data + (i * dev->item_size), src, dev->item_size);

        dev->tail = (dev->tail + 1) % dev->size;
    }

    return i;
}
