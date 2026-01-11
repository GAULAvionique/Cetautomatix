#include "comm_utils.h"
#include "protocol_defs.h"

/* =========================================================
 * CRC-16/CCITT-FALSE
 *  - Poly  : 0x1021
 *  - Init  : 0xFFFF
 *  - RefIn : false
 *  - RefOut: false
 *  - XorOut: 0x0000
 * ========================================================= */
uint16_t comm_crc16_ccitt(const void* data, uint16_t len) {
    const uint8_t* p = (const uint8_t*)data;
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)p[i] << 8;
        for (uint8_t b = 0; b < 8; b++) {
            if (crc & 0x8000) {
                crc = (uint16_t)((crc << 1) ^ 0x1021);
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

/* =========================================================
 * CRC-8/ATM (poly=0x07, init=0x00)
 * Utilisé pour la trame SAS -> GSE (UART) sur 1 octet de data.
 * ========================================================= */
uint8_t comm_crc8_atm(const void* data, uint16_t len) {
    const uint8_t* p = (const uint8_t*)data;
    uint8_t crc = 0x00;

    for (uint16_t i = 0; i < len; i++) {
        crc ^= p[i];
        for (uint8_t b = 0; b < 8; b++) {
            if (crc & 0x80) crc = (uint8_t)((crc << 1) ^ 0x07);
            else            crc = (uint8_t)(crc << 1);
        }
    }
    return crc;
}

/* =========================================================
 * UART byte-stuffing (SAS -> GSE)
 * Trame sur la ligne:
 *   '$' + (DATA/CRC échappés si besoin) + '$'
 *
 * - '$' est le délimiteur (start/end) sur 8 bits
 * - '\\' est l'octet d'échappement
 * - un byte spécial est envoyé comme: '\\' suivi de (byte ^ 0x20)
 * ========================================================= */
static void uart_push_escaped_byte(uint8_t byte, uint8_t* out, uint16_t* idx, uint16_t out_max) {
    if (*idx >= out_max) return;

    if (byte == SAS_UART_FLAG || byte == SAS_UART_ESC) {
        if ((*idx + 2u) > out_max) return;
        out[(*idx)++] = SAS_UART_ESC;
        out[(*idx)++] = (uint8_t)(byte ^ SAS_UART_ESC_XOR);
    } else {
        out[(*idx)++] = byte;
    }
}

/**
 * Construit la trame SAS->GSE (UART):
 *   Start  : '$' (8 bits)
 *   Data   : 1 byte (cmd_byte_t.byte)
 *   CRC8   : 1 byte (sur Data)
 *   End    : '$' (8 bits)
 *
 * Avec échappement (byte-stuffing) appliqué uniquement à Data et CRC.
 *
 * Retourne le nombre d'octets écrits dans out (0 si erreur).
 */
uint16_t comm_uart_build_sas_frame(uint8_t cmd_byte, uint8_t* out, uint16_t out_max) {
    if (!out || out_max < 4u) return 0;

    uint8_t crc = comm_crc8_atm(&cmd_byte, 1);

    uint16_t idx = 0;
    out[idx++] = SAS_UART_FLAG;

    uart_push_escaped_byte(cmd_byte, out, &idx, out_max);
    uart_push_escaped_byte(crc,      out, &idx, out_max);

    if (idx >= out_max) return 0;
    out[idx++] = SAS_UART_FLAG;

    return idx;
}

uint8_t comm_seq_next(void) {
    static uint8_t seq = 0;
    uint8_t ret = seq & 0x07;
    seq = (uint8_t)((seq + 1) & 0x07);
    return ret;
}
