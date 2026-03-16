#pragma once

#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include "ring_buffer.h"


#define CAN_RX_BUFFER_SIZE	112		// 14 bytes (id + dlc + data[8] + data[1]) * queue of 8
#define CAN_ID_NORMAL	 	0x120

typedef struct {
    uint32_t id;
    uint8_t  dlc;
    uint8_t  data[8];
} can_frame_t;

typedef struct {
	CAN_HandleTypeDef 	*hcan;
	CAN_TxHeaderTypeDef tx_header;
	ring_buffer_t 		rx_rb;
	can_frame_t 		rx_buffer_storage[CAN_RX_BUFFER_SIZE];
} can_t;


/*----------------------------
 On-time Functions
----------------------------*/
int8_t CAN_FIFO_Init(can_t *dev, CAN_HandleTypeDef *hcan);

int8_t CAN_FIFO_Send(can_t *dev, const uint8_t *package);
void CAN_FIFO_SetTXHeader(can_t *dev, uint32_t id, uint8_t len);
bool CAN_FIFO_ReadFrame(can_t *dev, can_frame_t *frame);
uint16_t CAN_FIFO_Available(can_t *dev);

/*----------------------------
 Interrupted Reception
----------------------------*/
void CAN_FIFO_RX0_IRQHandler_Callback(can_t *dev);
void CAN_FIFO_RX1_IRQHandler_Callback(can_t *dev);
void CAN_FIFO_SCE_IRQHandler_Callback(can_t *dev);
