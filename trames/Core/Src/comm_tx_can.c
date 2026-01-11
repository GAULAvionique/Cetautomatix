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

/* --------------------------------------------------------------------------
 * NEW: Commande compacte GSE -> ENGINE
 * Payload CAN: 1 byte (cmd_byte_t.byte) contenant SEQ/HB/C1..C4
 * -------------------------------------------------------------------------- */
int comm_tx_can_send_moteur_cmd(CAN_HandleTypeDef* hcan, uint8_t cmd_byte) {
    CAN_TxHeaderTypeDef tx = {0};
    tx.StdId = (CAN_ID_CMD_ENGINE & 0x7FFu);
    tx.IDE   = CAN_ID_STD;
    tx.RTR   = CAN_RTR_DATA;
    tx.DLC   = 1;

    uint8_t d[1] = { cmd_byte };

    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0U)
        return -2;

    uint32_t mb;
    return (HAL_CAN_AddTxMessage(hcan, &tx, d, &mb) == HAL_OK) ? 0 : -1;
}
