#include "GAUL_Drivers/uart_dma.h"


int8_t UART_DMA_Init(uart_dma_t *dev) {
	if(!dev) return -1;

    dev->old_pos = 0;
    dev->data_ready = false;

    return 0;
}

void UART_DMA_Start(uart_dma_t *dev) {
    HAL_UART_Receive_DMA(dev->huart, dev->dma_buffer, dev->dma_size);

    __HAL_UART_ENABLE_IT(dev->huart, UART_IT_IDLE);
}

void UART_DMA_Process(uart_dma_t *dev) {
    uint16_t pos = dev->dma_size - __HAL_DMA_GET_COUNTER(dev->huart->hdmarx);

    if(pos != dev->old_pos) {
        if(pos > dev->old_pos) {
            RingBuffer_Write(dev->rx_rb, &dev->dma_buffer[dev->old_pos], pos - dev->old_pos);
        } else {
            RingBuffer_Write(dev->rx_rb, &dev->dma_buffer[dev->old_pos], dev->dma_size - dev->old_pos);
            RingBuffer_Write(dev->rx_rb, &dev->dma_buffer[0], pos);
        }

        dev->old_pos = pos;
        dev->data_ready = true;
    }
}

void UART_DMA_IRQHandler(uart_dma_t *dev, UART_HandleTypeDef *huart) {
    if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(huart);

        UART_DMA_Process(dev);
    }

    if(__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) || __HAL_UART_GET_FLAG(huart, UART_FLAG_NE)  || __HAL_UART_GET_FLAG(huart, UART_FLAG_FE)) {
		__HAL_UART_CLEAR_FEFLAG(huart);
		__HAL_UART_CLEAR_NEFLAG(huart);
		__HAL_UART_CLEAR_OREFLAG(huart);
    }
}
