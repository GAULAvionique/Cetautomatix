#include "comm_tx_rf.h"

/*
 * Ce fichier sert à transmettre sur le lien RF (RFD900x) via UART.
 *
 *
 * - Les commandes SAS->GSE ont  un format:
 *     Start '$' (8 bits)
 *     Data  (1 byte: SEQ/HB/C1..C4)
 *     CRC8  (1 byte)
 *     End   '$' (8 bits)
 *   + échappement (byte-stuffing) pour éviter que '$' apparaisse dans Data/CRC.
 */

/* --------------------------------------------------------------------------
 * Envoi commande SAS -> GSE (UART vers modem RF)
 * -------------------------------------------------------------------------- */
int comm_tx_rf_send_sas_cmd(UART_HandleTypeDef* huart,
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
  uint16_t n = comm_uart_build_sas_frame(cmd_byte, frame, (uint16_t)sizeof(frame));
  if (n == 0u) return -2;

  return (HAL_UART_Transmit(huart, frame, n, timeout) == HAL_OK) ? 0 : -1;
}
