#include "comm_utils.h"
#include "stm32f4xx_hal_can.h"
#include <stdarg.h> 
#include <stdio.h>
#include <string.h> 
#include "comm_tx.h"
#include <stdint.h>

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

    if (byte == TRAMES_FLAG || byte == SAS_UART_ESC) {
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
uint16_t comm_uart_build_cmd_frame(uint8_t cmd_byte, uint8_t* out, uint16_t out_max) {
    if (!out || out_max < 4u) return 0;

    uint8_t crc = comm_crc8_atm(&cmd_byte, 1);

    uint16_t idx = 0;
    out[idx++] = TRAMES_FLAG;

    uart_push_escaped_byte(cmd_byte, out, &idx, out_max);
    uart_push_escaped_byte(crc,      out, &idx, out_max);

    if (idx >= out_max) return 0;
    out[idx++] = TRAMES_FLAG;

    return idx;
}

/**
 * Construit la trame UART d'un status moteur déjà packé.
 * Entrée:
 *   raw_frame[10] = trame brute produite par motor_status_pack():
 *     [0] = '$'
 *     [1..8] = payload (8 octets)
 *     [9] = '$'
 * La fonction:
 *   - extrait le payload [1..8]
 *   - calcule un CRC8 (ATM) sur ces 8 octets
 *   - construit la trame UART:
 *       '$' + payload_escaped + crc_escaped + '$'
 *   où payload_escaped et crc_escaped sont échappés si nécessaire
 *   (byte-stuffing pour '$' ou '\' ).
 * Sortie:
 *   out = trame UART prête à être envoyée.
 * Taille max en pire cas:
 *   start '$'                    = 1
 *   payload (8 octets échappés)  = 16
 *   crc (1 octet échappé)        = 2
 *   end '$'                      = 1
 *   total max                    = 20 octets
 * Retour:
 *   nombre d'octets écrits dans out
 *   0 si erreur
 */
uint16_t comm_uart_build_motor_status_frame(const uint8_t raw_frame[10],
                                            uint8_t* out,
                                            uint16_t out_max)
{
    if (!raw_frame || !out || out_max < 11u) return 0;
    if ((raw_frame[0] != TRAMES_FLAG) || (raw_frame[9] != TRAMES_FLAG)) return 0;

    /* raw_frame = ['$' payload[8] '$'] */

    const uint8_t* payload = &raw_frame[1];

    /* CRC sur les 8 bytes de payload */
    uint8_t crc = comm_crc8_atm(payload, 8);

    uint16_t idx = 0;

    /* start flag */
    out[idx++] = TRAMES_FLAG;

    /* payload échappé */
    for (uint8_t i = 0; i < 8; i++) {
        uart_push_escaped_byte(payload[i], out, &idx, out_max);
    }

    /* CRC échappé */
    uart_push_escaped_byte(crc, out, &idx, out_max);

    if (idx >= out_max)
        return 0;

    /* end flag */
    out[idx++] = TRAMES_FLAG;

    return idx;
}

uint8_t comm_seq_next(void) {
    static uint8_t seq = 0;
    uint8_t ret = seq & 0x07;
    seq = (uint8_t)((seq + 1) & 0x07);
    return ret;
}

/* ------------------------------------------------------------- */
/* DEBUG PRINTS */
/* ------------------------------------------------------------- */

void dbg_puts(UART_HandleTypeDef *huart, const char *s)
{
    HAL_UART_Transmit(huart, (uint8_t*)s, strlen(s), 100);
}

void dbg_printf(UART_HandleTypeDef *huart, const char *fmt, ...)
{
    char buf[180];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    HAL_UART_Transmit(huart, (uint8_t*)buf, strlen(buf), 100);
}

/* ------------------------------------------------------------- */
/* WAIT CAN RX */
/* ------------------------------------------------------------- */

bool wait_can_rx(CAN_HandleTypeDef *hcan,
                        CAN_RxHeaderTypeDef *rxh,
                        uint8_t *data,
                        uint32_t timeout_ms)
{
    uint32_t t0 = HAL_GetTick();

    while (HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) == 0)
    {
        if ((HAL_GetTick() - t0) > timeout_ms)
            return false;
    }

    return HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, rxh, data) == HAL_OK;
}

/* ------------------------------------------------------------- */
/* UART UNSTUFF */
/* ------------------------------------------------------------- */

bool uart_unstuff_between_flags(const uint8_t *frame,
                                       uint16_t frame_len,
                                       uint8_t *decoded,
                                       uint16_t *decoded_len)
{
    if (!frame || !decoded || !decoded_len)
        return false;

    if (frame_len < 2)
        return false;

    if (frame[0] != TRAMES_FLAG || frame[frame_len-1] != TRAMES_FLAG)
        return false;

    uint16_t out_i = 0;

    for (uint16_t i = 1; i < frame_len-1; i++)
    {
        if (frame[i] == SAS_UART_ESC)
        {
            if ((i+1) >= frame_len-1)
                return false;

            decoded[out_i++] = frame[i+1] ^ SAS_UART_ESC_XOR;
            i++;
        }
        else
        {
            decoded[out_i++] = frame[i];
        }
    }

    *decoded_len = out_i;
    return true;
}

/* ------------------------------------------------------------- */
/* PARSE UART COMMAND */
/* ------------------------------------------------------------- */

bool parse_uart_cmd_frame(const uint8_t *frame,
                                 uint16_t frame_len,
                                 uint8_t *cmd_byte_out)
{
    uint8_t decoded[4];
    uint16_t decoded_len = 0;

    if (!uart_unstuff_between_flags(frame, frame_len, decoded, &decoded_len))
        return false;

    if (decoded_len != 2)
        return false;

    uint8_t cmd = decoded[0];
    uint8_t crc = decoded[1];

    if (comm_crc8_atm(&cmd,1) != crc)
        return false;

    *cmd_byte_out = cmd;

    return true;
}

/* ------------------------------------------------------------- */
/* PARSE UART STATUS */
/* ------------------------------------------------------------- */

bool parse_uart_motor_status_frame(const uint8_t *frame,
                                          uint16_t frame_len,
                                          motor_status_t *st_out)
{
    uint8_t decoded[12];
    uint16_t decoded_len = 0;

    uint8_t raw[10];

    if (!uart_unstuff_between_flags(frame,frame_len,decoded,&decoded_len))
        return false;

    if (decoded_len != 9)
        return false;

    if (comm_crc8_atm(decoded,8) != decoded[8])
        return false;

    raw[0] = TRAMES_FLAG;

    memcpy(&raw[1],decoded,8);

    raw[9] = TRAMES_FLAG;

    return motor_status_unpack(st_out,raw);
}

/* ------------------------------------------------------------- */
/* EXTRACT COMMAND STATES */
/* ------------------------------------------------------------- */

uint8_t cmd_states_from_cmd_byte(uint8_t cmd_byte)
{
    return (cmd_byte >> 4) & 0x0F;
}

/* ------------------------------------------------------------- */
/* RECEIVE MOTOR STATUS FROM CAN */
/* ------------------------------------------------------------- */

int recv_motor_status_from_can(CAN_HandleTypeDef *hcan,
                                      motor_status_t *st,
                                      uint32_t timeout_ms)
{
    CAN_RxHeaderTypeDef rxh;
    uint8_t data[8];

    uint8_t raw[10];

    bool got_seg0 = false;
    bool got_seg1 = false;

    uint32_t t0 = HAL_GetTick();

    while ((!got_seg0 || !got_seg1) &&
           ((HAL_GetTick() - t0) <= timeout_ms))
    {
        if (!wait_can_rx(hcan,&rxh,data,10))
            continue;

        if (rxh.StdId == (CAN_ID_STATUS_MIN & 0x7FF) && rxh.DLC == 8)
        {
            memcpy(&raw[0],data,8);
            got_seg0 = true;
        }
        else if (rxh.StdId == ((CAN_ID_STATUS_MIN+1) & 0x7FF) && rxh.DLC == 2)
        {
            raw[8] = data[0];
            raw[9] = data[1];
            got_seg1 = true;
        }
    }

    if (!got_seg0 || !got_seg1)
        return -1;
    return motor_status_unpack(st,raw) ? 0 : -1;
}

/* ------------------------------------------------------------- */
/* MAIN TEST */
/* ------------------------------------------------------------- */
void run_end_to_end_test_once(CAN_HandleTypeDef* hcan,
                              UART_HandleTypeDef* huart,
                              bool *hb,
                              uint32_t timeout_ms)
{
    CAN_RxHeaderTypeDef canRxHdr;
    uint8_t can_rxd[8];

    uint8_t seq_cmd;
    cmd_byte_t sas_cmd;

    uint8_t sas_uart_cmd_frame[8];
    uint16_t sas_uart_cmd_len;

    uint8_t gse_received_cmd = 0;

    motor_status_t motor_status_tx;
    motor_status_t gse_status_rx;
    motor_status_t sas_status_rx;

    uint8_t gse_uart_status_raw[10];
    uint8_t gse_uart_status_frame[20];
    uint16_t gse_uart_status_len;

    if (!hcan || !huart || !hb) {
        return;
    }

    dbg_puts(huart, "\r\n=== TEST PROTO COMPLET ===\r\n");

    /* ---------------- SAS -> GSE UART ---------------- */

    *hb = !(*hb);
    seq_cmd = comm_seq_next();

    sas_cmd = cmd_make(seq_cmd, *hb, true, false, false, false);

    sas_uart_cmd_len =
        comm_uart_build_cmd_frame(sas_cmd.byte,
                                  sas_uart_cmd_frame,
                                  sizeof(sas_uart_cmd_frame));

    comm_tx_rf_send_gse_cmd(huart, 50, sas_cmd.byte);

    dbg_printf(huart, "SAS->GSE UART cmd ok : cmd=0x%02X, len=%u\r\n",
               sas_cmd.byte, sas_uart_cmd_len);

    parse_uart_cmd_frame(sas_uart_cmd_frame,
                         sas_uart_cmd_len,
                         &gse_received_cmd);

    dbg_printf(huart, "GSE a recu cmd = 0x%02X\r\n", gse_received_cmd);

    /* ---------------- GSE -> MOTOR CAN ---------------- */

    comm_tx_can_send_moteur_cmd(hcan, gse_received_cmd);

    wait_can_rx(hcan, &canRxHdr, can_rxd, timeout_ms);

    dbg_printf(huart, "MOTOR a recu CAN cmd = 0x%02X\r\n", can_rxd[0]);

    /* ---------------- MOTOR STATUS ---------------- */

    motor_status_tx =
        motor_status_make(comm_seq_next(),
                          *hb,
                          true,
                          5,
                          cmd_states_from_cmd_byte(can_rxd[0]),
                          1234,
                          1, 1, 0, 0, 0,
                          (uint16_t)HAL_GetTick());

    comm_status_tx_can_send_gse_cmd(hcan, &motor_status_tx);

    recv_motor_status_from_can(hcan, &gse_status_rx, timeout_ms);

    dbg_printf(huart, "GSE a recu status motor: seq=%u hb=%u batt=%u p=%u cmd=0x%X ts=%u\r\n",
               gse_status_rx.seq,
               gse_status_rx.hb,
               gse_status_rx.battery_percent,
               gse_status_rx.n2o_pressure,
               gse_status_rx.command_states,
               gse_status_rx.timestamp_ms);

    /* ---------------- GSE -> SAS UART ---------------- */

    motor_status_pack(&gse_status_rx, gse_uart_status_raw);

    gse_uart_status_len =
        comm_uart_build_motor_status_frame(gse_uart_status_raw,
                                           gse_uart_status_frame,
                                           sizeof(gse_uart_status_frame));

    comm_status_tx_rf_send_sas_cmd(huart, 50, &gse_status_rx);

    parse_uart_motor_status_frame(gse_uart_status_frame,
                                  gse_uart_status_len,
                                  &sas_status_rx);

    dbg_printf(huart, "SAS a recu status: seq=%u hb=%u batt=%u p=%u cmd=0x%X ts=%u\r\n",
               sas_status_rx.seq,
               sas_status_rx.hb,
               sas_status_rx.battery_percent,
               sas_status_rx.n2o_pressure,
               sas_status_rx.command_states,
               sas_status_rx.timestamp_ms);

    dbg_puts(huart, "OK: chaine SAS -> GSE -> MOTOR -> GSE -> SAS valide\r\n");
}