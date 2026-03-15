#pragma once

#include <stdint.h>
#include <stddef.h>


typedef struct {
    void 				*buffer;
    uint16_t 			size;		// slot size
    uint16_t 			item_size;	// element size (uint8_t, etc)
    volatile uint16_t 	head;
    volatile uint16_t 	tail;
} ring_buffer_t;


int8_t RingBuffer_Init(ring_buffer_t *dev, void *buffer, uint16_t size, uint16_t item_size);

/*----------------------------
 On-time Functions
----------------------------*/
uint16_t RingBuffer_Write(ring_buffer_t *dev, const void *data, uint16_t len);
uint16_t RingBuffer_Read(ring_buffer_t *dev, void *data, uint16_t len);
uint16_t RingBuffer_Available(ring_buffer_t *dev);
