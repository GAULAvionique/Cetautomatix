#include <GAUL/Low_Level/ring_buffer.h>
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
    if (len == 0) return 0;

    uint16_t available = dev->size - RingBuffer_Available(dev) - 1;
    if (len > available) len = available;

    const uint8_t *p_data = (const uint8_t *)data;
    uint16_t bytes_to_end = dev->size - dev->head;

    if (len <= bytes_to_end) {
        memcpy((uint8_t*)dev->buffer + (dev->head * dev->item_size), p_data, len * dev->item_size);
        dev->head = (dev->head + len) % dev->size;
    } else {
        memcpy((uint8_t*)dev->buffer + (dev->head * dev->item_size), p_data, bytes_to_end * dev->item_size);
        memcpy((uint8_t*)dev->buffer, p_data + (bytes_to_end * dev->item_size), (len - bytes_to_end) * dev->item_size);
        dev->head = len - bytes_to_end;
    }

    return len;
}

uint16_t RingBuffer_Read(ring_buffer_t *dev, void *data, uint16_t len) {
    if (len == 0) return 0;

    uint16_t available = RingBuffer_Available(dev);
    if (len > available) len = available;

    uint8_t *p_data = (uint8_t *)data;
    uint16_t bytes_to_end = dev->size - dev->tail;

    if (len <= bytes_to_end) {
        memcpy(p_data, (uint8_t*)dev->buffer + (dev->tail * dev->item_size), len * dev->item_size);
        dev->tail = (dev->tail + len) % dev->size;
    } else {
        memcpy(p_data, (uint8_t*)dev->buffer + (dev->tail * dev->item_size), bytes_to_end * dev->item_size);
        memcpy(p_data + (bytes_to_end * dev->item_size), (uint8_t*)dev->buffer, (len - bytes_to_end) * dev->item_size);
        dev->tail = len - bytes_to_end;
    }

    return len;
}
