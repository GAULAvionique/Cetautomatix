#pragma once


#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "utils.h"


status_t COMM_Init(CAN_HandleTypeDef *dev, CAN_FilterTypeDef *filter);

status_t COMM_CAN_TXD(CAN_HandleTypeDef *dev, uint8_t *data, size_t len);
status_t COMM_CAN_RXD(CAN_HandleTypeDef *dev, uint8_t *data, size_t len);
//status_t COMM_RFD_TX(CAN_HandleTypeDef *dev, uint8_t *data, size_t len);
//status_t COMM_RFD_RX(CAN_HandleTypeDef *dev, uint8_t *data, size_t len);
