#pragma once

#include <stdint.h>
#include <stddef.h>
#include "stm32f1xx_hal.h"


typedef struct {
	UART_HandleTypeDef 	*UARTx;
} rfd900x_t;


int8_t RFD900x_Init(rfd900x_t *dev);

int8_t RFD900x_Send(rfd900x_t *dev, const uint8_t *package, size_t len, uint32_t timeout);
int8_t RFD900x_Receive(rfd900x_t *dev, uint8_t *package, size_t len, uint32_t timeout);
