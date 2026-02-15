#pragma once
#include <stdint.h>
#include "protocol_defs.h"
#include "stm32f4xx_hal.h"

int comm_tx_can_send_estop(CAN_HandleTypeDef* hcan, uint8_t reason);

int comm_tx_can_send_moteur_cmd(CAN_HandleTypeDef* hcan, uint8_t cmd_byte);


