#pragma once
#include <stdint.h>
#include "protocol_defs.h"
#include "comm_utils.h"
#include "stm32f4xx_hal.h"

int comm_tx_can_send_estop(CAN_HandleTypeDef* hcan, uint8_t reason);

int comm_tx_can_send_moteur_cmd(CAN_HandleTypeDef* hcan, uint8_t cmd_byte);

/*
 * Cette fonction sert à transmettre sur le lien RF (RFD900x) via UART.
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

int comm_hal_can_get_free_level(CAN_HandleTypeDef* hcan);

HAL_StatusTypeDef comm_hal_can_add_tx(CAN_HandleTypeDef* hcan,
                                      CAN_TxHeaderTypeDef* tx,
                                      uint8_t* d,
                                      uint32_t* mb);

HAL_StatusTypeDef comm_hal_uart_transmit(UART_HandleTypeDef* huart,
                                         uint8_t* data,
                                         uint16_t len,
                                         uint32_t timeout);
