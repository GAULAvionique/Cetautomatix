#pragma once

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "utils.h"


typedef struct {
    USART_TypeDef 	*UARTx;
} RFD900x_t;

status_t RFD900x_Init(RFD900x_t *dev);

status_t RFD900x_Send(RFD900x_t *dev);
status_t RFD900x_Receive(RFD900x_t *dev);
