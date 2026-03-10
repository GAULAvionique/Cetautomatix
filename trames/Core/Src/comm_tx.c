#include <comm_tx.h>

#ifndef UNIT_TEST
int comm_hal_can_get_free_level(CAN_HandleTypeDef* hcan)
{
    return (int)HAL_CAN_GetTxMailboxesFreeLevel(hcan);
}

HAL_StatusTypeDef comm_hal_can_add_tx(CAN_HandleTypeDef* hcan,
                                      CAN_TxHeaderTypeDef* tx,
                                      uint8_t* d,
                                      uint32_t* mb)
{
    return HAL_CAN_AddTxMessage(hcan, tx, d, mb);
}

HAL_StatusTypeDef comm_hal_uart_transmit(UART_HandleTypeDef* huart,
                                         uint8_t* data,
                                         uint16_t len,
                                         uint32_t timeout)
{
    return HAL_UART_Transmit(huart, data, len, timeout);
}
#endif

int comm_tx_can_send_estop(CAN_HandleTypeDef* hcan, uint8_t reason) {
    CAN_TxHeaderTypeDef tx = {0};
    tx.StdId = (CAN_ID_ESTOP & 0x7FF);  // 11 bits
    tx.IDE   = CAN_ID_STD;
    tx.RTR   = CAN_RTR_DATA;
    tx.DLC   = 8;

    uint8_t d[8] = { 'E','S','T','P', reason, 0,0,0 };

    if (comm_hal_can_get_free_level(hcan) == 0U)
        return -2;

    uint32_t mb;
    return (comm_hal_can_add_tx(hcan, &tx, d, &mb) == HAL_OK) ? 0 : -1;
}

/* --------------------------------------------------------------------------
 * NEW: Commande compacte GSE -> MOTEUR
 * Payload CAN: 1 byte (cmd_byte_t.byte) contenant SEQ/HB/C1..C4
 * -------------------------------------------------------------------------- */
int comm_tx_can_send_moteur_cmd(CAN_HandleTypeDef* hcan, uint8_t cmd_byte) {
    CAN_TxHeaderTypeDef tx = {0};
    tx.StdId = (CAN_ID_CMD_MOTOR & 0x7FFu);
    tx.IDE   = CAN_ID_STD;
    tx.RTR   = CAN_RTR_DATA;
    tx.DLC   = 1;

    uint8_t d[1] = { cmd_byte };

    if (comm_hal_can_get_free_level(hcan)== 0U)
        return -2;

    uint32_t mb;
    return (comm_hal_can_add_tx(hcan, &tx, d, &mb) == HAL_OK) ? 0 : -1;
}

/* --------------------------------------------------------------------------
 * Envoi commande SAS -> GSE (UART vers modem RF)
 * -------------------------------------------------------------------------- */
int comm_tx_rf_send_gse_cmd(UART_HandleTypeDef* huart,
                            uint32_t timeout,
                            uint8_t cmd_byte)
{
  /*
   * Taille max:
   *   '$' + (DATA escape?) + (CRC escape?) + '$'
   * Dans le pire cas DATA et CRC sont échappés => +2+2.
   * Donc max = 1 + 2 + 2 + 1 = 6 bytes.
   */
  uint8_t frame[8];
  uint16_t n = comm_uart_build_cmd_frame(cmd_byte, frame, (uint16_t)sizeof(frame));
  if (n == 0u) return -2;

  return (comm_hal_uart_transmit(huart, frame, n, timeout) == HAL_OK) ? 0 : -1;
}

/* --------------------------------------------------------------------------
 * Envoi STATUS GSE -> SAS (UART via modem RF)
 * -------------------------------------------------------------------------- */
int comm_status_tx_rf_send_sas_cmd(UART_HandleTypeDef* huart,
                                   uint32_t timeout,
                                   const motor_status_t* st)
{
  if (!huart || !st)
      return -2;

  /* Construire la trame brute 10 bytes */
  uint8_t raw[10];
  motor_status_pack(st, raw);

  /*
   * Appliquer le byte-stuffing UART
   * pire cas = 20 bytes
   * '$' + payload(8 échappés max=16) + CRC(échappé max=2) + '$'
   */
  uint8_t frame[20];
  uint16_t n = comm_uart_build_motor_status_frame(raw,
                                                  frame,
                                                  (uint16_t)sizeof(frame));

  if (n == 0u)
      return -2;

  return (comm_hal_uart_transmit(huart, frame, n, timeout) == HAL_OK) ? 0 : -1;
}


/* --------------------------------------------------------------------------
 * Envoi STATUS MOTEUR -> GSE via CAN
 * (trame 10 bytes segmentée en 2 CAN frames)
 * -------------------------------------------------------------------------- */
int comm_status_tx_can_send_gse_cmd(CAN_HandleTypeDef* hcan,
                                    const motor_status_t* st)
{
    if (!hcan || !st)
        return -2;

    uint8_t raw[10];
    motor_status_pack(st, raw);

    CAN_TxHeaderTypeDef tx = {0};
    tx.StdId = (CAN_ID_STATUS_MIN & 0x7FFu);
    tx.IDE   = CAN_ID_STD;
    tx.RTR   = CAN_RTR_DATA;

    uint32_t mb;

    if (comm_hal_can_get_free_level(hcan) == 0U)
        return -2;

    /* Frame 1 : 8 bytes */
    tx.DLC = 8;

    if (comm_hal_can_add_tx(hcan, &tx, raw, &mb) != HAL_OK)
        return -1;

    if (comm_hal_can_get_free_level(hcan) == 0U)
        return -2;

    /* Frame 2 : 2 bytes restants */
    uint8_t d2[2];
    d2[0] = raw[8];
    d2[1] = raw[9];

    tx.StdId = (CAN_ID_STATUS_MIN + 1) & 0x7FFu;
    tx.DLC   = 2;

    return (comm_hal_can_add_tx(hcan, &tx, d2, &mb) == HAL_OK) ? 0 : -1;
}