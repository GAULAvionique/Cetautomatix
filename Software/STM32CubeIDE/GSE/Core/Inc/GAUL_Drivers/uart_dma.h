#pragma once

#include "stm32f1xx_hal.h"
#include "ring_buffer.h"
#include <stdbool.h>


typedef struct {
    UART_HandleTypeDef	*huart;
    uint8_t 			*dma_buffer;
    uint16_t 			dma_size;
    uint16_t 			old_pos;
    ring_buffer_t 		*rx_rb;
    volatile bool 		data_ready;
} uart_dma_t;

int8_t UART_DMA_Init(uart_dma_t *dev);

/*----------------------------
 On-time Functions
----------------------------*/
void UART_DMA_Start(uart_dma_t *dev);
void UART_DMA_Process(uart_dma_t *dev);
void UART_DMA_IRQHandler(uart_dma_t *dev, UART_HandleTypeDef *huart);
