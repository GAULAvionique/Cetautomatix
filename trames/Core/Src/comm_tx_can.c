#include "comm_tx_can.h"

int comm_tx_can_send_estop(CAN_HandleTypeDef* hcan, uint8_t reason) {
    CAN_TxHeaderTypeDef tx = {0};
    tx.StdId = (CAN_ID_ESTOP & 0x7FF);  // 11 bits
    tx.IDE   = CAN_ID_STD;
    tx.RTR   = CAN_RTR_DATA;
    tx.DLC   = 8;

    uint8_t d[8] = { 'E','S','T','P', reason, 0,0,0 };

    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0U)
        return -2;

    uint32_t mb;
    return (HAL_CAN_AddTxMessage(hcan, &tx, d, &mb) == HAL_OK) ? 0 : -1;
}

int comm_tx_can_send_cmd_short(CAN_HandleTypeDef* hcan, uint8_t op, uint8_t seq, const uint8_t args[6]) {
    CAN_TxHeaderTypeDef tx = {0};
    tx.StdId = (CAN_ID_CMD_SHORT & 0x7FF); // 11 bits
    tx.IDE   = CAN_ID_STD;
    tx.RTR   = CAN_RTR_DATA;
    tx.DLC   = 8;

    uint8_t d[8] = {
        op, seq,
        args ? args[0] : 0,
        args ? args[1] : 0,
        args ? args[2] : 0,
        args ? args[3] : 0,
        args ? args[4] : 0,
        args ? args[5] : 0
    };

    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0U)
        return -2;

    uint32_t mb;
    return (HAL_CAN_AddTxMessage(hcan, &tx, d, &mb) == HAL_OK) ? 0 : -1;
}

static inline int comm_tx_can_set_valve(CAN_HandleTypeDef* hcan, uint8_t seq, uint8_t valve_id, uint8_t state) {
  uint8_t a[6] = { valve_id, state, 0,0,0,0 };
  return comm_tx_can_send_cmd_short(hcan, OP_SET_VALVE, seq, a);
}

static inline int comm_tx_can_arm(CAN_HandleTypeDef* hcan, uint8_t seq) {
  uint8_t a[6] = {0};
  return comm_tx_can_send_cmd_short(hcan, OP_ARM, seq, a);
}

int comm_tx_can_purge_start(CAN_HandleTypeDef* hcan, uint8_t seq) {
    uint8_t a[6] = {0};
    return comm_tx_can_send_cmd_short(hcan, OP_PURGE_START, seq, a);
}
int comm_tx_can_purge_stop(CAN_HandleTypeDef* hcan, uint8_t seq) {
    uint8_t a[6] = {0};
    return comm_tx_can_send_cmd_short(hcan, OP_PURGE_STOP, seq, a);
}
int comm_tx_can_fill_start(CAN_HandleTypeDef* hcan, uint8_t seq) {
    uint8_t a[6] = {0};
    return comm_tx_can_send_cmd_short(hcan, OP_FILL_START, seq, a);
}
int comm_tx_can_fill_stop(CAN_HandleTypeDef* hcan, uint8_t seq) {
    uint8_t a[6] = {0};
    return comm_tx_can_send_cmd_short(hcan, OP_FILL_STOP, seq, a);
}
