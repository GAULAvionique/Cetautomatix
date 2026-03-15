#pragma once

#include "stm32f1xx_hal.h"
#include <stdbool.h>


#define CAN_ID_NORMAL 0x120

typedef struct {
	CAN_HandleTypeDef 	*hcan;
	CAN_TxHeaderTypeDef tx_header;
} can_t;

// callback
typedef void (*CAN_RxCallback)(uint32_t StdId, uint8_t *data, uint8_t len);


/*----------------------------
 On-time Functions
----------------------------*/
int8_t CAN_FIFO_Init(can_t *dev, CAN_HandleTypeDef *hcan);

int8_t CAN_FIFO_Send(can_t *dev, const uint8_t *data);
void CAN_FIFO_SetTXHeader(can_t *dev, uint32_t id, uint8_t len);

/*----------------------------
 Callback
----------------------------*/
void CAN_FIFO_SetRxCallback(can_t *dev, CAN_RxCallback callback);

/*----------------------------
 Interrupted Reception
----------------------------*/
void CAN_FIFO_RX0_IRQHandler_Callback(can_t *dev);
void CAN_FIFO_RX1_IRQHandler_Callback(can_t *dev);
void CAN_FIFO_SCE_IRQHandler_Callback(can_t *dev);
