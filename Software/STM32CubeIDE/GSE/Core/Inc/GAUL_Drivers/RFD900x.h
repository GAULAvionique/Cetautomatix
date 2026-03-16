#pragma once

#include "uart_dma.h"
#include "ring_buffer.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define RFD_DMA_RX_SIZE	32
#define RFD_RING_SIZE	64

typedef struct {
    uart_dma_t 		*uart;
    ring_buffer_t 	rb;
	uint8_t 		dma_buffer[RFD_DMA_RX_SIZE];
	uint8_t 		ring_mem[RFD_RING_SIZE];
} rfd900x_t;


int8_t RFD900x_Init(rfd900x_t *dev, UART_HandleTypeDef *huart);

/*----------------------------
 On-time Functions
----------------------------*/
int8_t RFD900x_Send(rfd900x_t *dev, const uint8_t *package, size_t len, uint32_t timeout);
bool RFD900x_ReadFrame(rfd900x_t *dev, uint8_t *package, uint16_t frame_size);
uint16_t RFD900x_Available(rfd900x_t *dev);
