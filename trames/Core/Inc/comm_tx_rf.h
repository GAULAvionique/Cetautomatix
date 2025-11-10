#pragma once
#include <stdint.h>
#include "protocol_defs.h"
#include "comm_utils.h"
#include "stm32f4xx_hal.h"

/**
 * Construit et ENVOIE une trame RF:
 *   payload_logique = [ver|src|dst|op|seq|ts_ms(4)|len(2)|payload...|crc16(2)]
 *   framing = COBS(payload_logique) + 0x00 (d�limiteur)
 *
 * @param huart   UART du RFD900x
 * @param timeout timeout HAL_UART_Transmit en ms
 * @return 0 si OK, <0 sinon
 */
int comm_tx_rf_send(UART_HandleTypeDef* huart, uint32_t timeout,
                    node_id_t src, node_id_t dst, uint8_t op,
                    uint8_t seq, uint32_t ts_ms,
                    const uint8_t* payload, uint16_t len);

