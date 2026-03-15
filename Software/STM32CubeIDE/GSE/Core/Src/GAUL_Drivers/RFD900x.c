#include <GAUL_Drivers/rfd900x.h>


int8_t RFD900x_Init(rfd900x_t *dev) {
	if(dev == NULL) return -1;
	if(dev->UARTx == NULL) return -1;

	return 0;
}

int8_t RFD900x_Send(rfd900x_t *dev, const uint8_t *package, size_t len, uint32_t timeout) {
	if(!dev || package == NULL || len <= 0 || timeout <= 0) return -1;

	if(HAL_UART_Transmit(dev->UARTx, package, len, timeout) != HAL_OK) return -1;

	return 0;
}

int8_t RFD900x_Receive(rfd900x_t *dev, uint8_t *package, size_t len, uint32_t timeout) {
	if(!dev || package == NULL || len <= 0 || timeout <= 0) return -1;

	if(HAL_UART_Receive(dev->UARTx, package, len, timeout) != HAL_OK) return -1;

	return 0;
}


/*
uint8_t RFD900_Init(RFD900 *devRFD) {

    devRFD->header = 0x00;
    devRFD->data = NULL;
    devRFD->crc = 0x00;
    devRFD->size = 0x00;
    return 1;
}

uint8_t RFD900_Send(RFD900 *devRFD) {

    uint8_t delim = '$';
    uint8_t crc_delim = '*';
    uint8_t new_line = '\n';

    USART_TX(devRFD->USARTx, &delim, 1); // Start
    USART_TX(devRFD->USARTx, &devRFD->header, 1);
    USART_TX(devRFD->USARTx, devRFD->data, devRFD->size);
    USART_TX(devRFD->USARTx, &crc_delim, 1); // CRC
    USART_TX(devRFD->USARTx, devRFD->crc, 2);
    USART_TX(devRFD->USARTx, &new_line, 1); // End
    return 1; // OK
}
*/
