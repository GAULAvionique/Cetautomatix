#include <GAUL_Drivers/rfd900x.h>


int8_t RFD900x_Init(rfd900x_t *dev, UART_HandleTypeDef *huart) {
    if(!dev || !huart) return -1;

    RingBuffer_Init(&dev->rb, dev->ring_mem, RFD_RING_SIZE, sizeof(uint8_t));

    dev->uart->huart = huart;
    dev->uart->dma_buffer = dev->dma_buffer;
    dev->uart->dma_size = RFD_DMA_RX_SIZE;
    dev->uart->rx_rb = &dev->rb;

    UART_DMA_Init(dev->uart);
    UART_DMA_Start(dev->uart);

    return 0;
}

int8_t RFD900x_Send(rfd900x_t *dev, const uint8_t *package, size_t len, uint32_t timeout) {
	if(!dev || !package || len == 0) return -1;

	if(HAL_UART_Transmit(dev->uart->huart, (uint8_t*)package, len, timeout) != HAL_OK) return -1;

	return 0;
}

static int16_t RFD900x_Receive(rfd900x_t *dev, uint8_t *package, size_t len) {
    if(!dev || !package || len == 0) return -1;

    return RingBuffer_Read(dev->uart->rx_rb, package, len);
}

uint16_t RFD900x_Available(rfd900x_t *dev) {
	return RingBuffer_Available(dev->uart->rx_rb);
}

bool RFD900x_ReadFrame(rfd900x_t *dev, uint8_t *package, uint16_t frame_size) {
    if(RFD900x_Available(dev) < frame_size) return false;

    int16_t n = RFD900x_Receive(dev, package, frame_size);
    return (n == frame_size);
}
