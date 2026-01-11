#pragma once
#include <stdint.h>
#include "protocol_defs.h"
#include "comm_utils.h"
#include "stm32f4xx_hal.h"

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
                            uint8_t cmd_byte);

