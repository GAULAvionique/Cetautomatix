#pragma once
#include <stdint.h>
#include "protocol_defs.h"
#include "stm32f4xx_hal.h"

int comm_tx_can_send_estop(CAN_HandleTypeDef* hcan, uint8_t reason);
/* data = [op][seq][a0][a1][a2][a3][a4][a5] sur CAN_ID_CMD_SHORT */
int comm_tx_can_send_cmd_short(CAN_HandleTypeDef* hcan, uint8_t op, uint8_t seq, const uint8_t args[6]);


static inline int comm_tx_can_set_valve(CAN_HandleTypeDef* hcan, uint8_t seq, uint8_t valve_id, uint8_t state);
static inline int comm_tx_can_arm(CAN_HandleTypeDef* hcan, uint8_t seq);
int comm_tx_can_purge_start(CAN_HandleTypeDef* hcan, uint8_t seq);
int comm_tx_can_purge_stop(CAN_HandleTypeDef* hcan, uint8_t seq);
int comm_tx_can_fill_start(CAN_HandleTypeDef* hcan, uint8_t seq);
int comm_tx_can_fill_stop(CAN_HandleTypeDef* hcan, uint8_t seq);
