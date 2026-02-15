#include "GAUL_Drivers/RFD900x.h"


status_t RFD900x_Init(RFD900x_t *dev) {
	if(dev == NULL) return STATUS_FAIL;
	if(dev->UARTx == NULL) return STATUS_FAIL;

	return STATUS_OK;
}

status_t RFD900x_Send(RFD900x_t *dev) {

	return STATUS_OK;
}

status_t RFD900x_Receive(RFD900x_t *dev) {

	return STATUS_OK;
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
