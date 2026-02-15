#pragma once
#include <stdint.h>
#include <stdbool.h>

/* =========================
 * Identifiants de noeuds
 * ========================= */
typedef enum {
  NODE_STATION = 0x01,
  NODE_GSE     = 0x02,
  NODE_ENGINE  = 0x03,
} node_id_t;

/* =========================
 * Version protocole
 * ========================= */
enum { PROTO_VER = 0x03 };  // format commandes mis à jour


/* =====================================================================
 * Commande compacte (1 byte) - utilisée pour :
 *  - SAS -> GSE (UART): payload = 1 byte
 *  - GSE -> ENGINE (CAN): data[0] = 1 byte
 * Layout:
 *   bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
 *    C4   C3   C2   C1   HB  SEQ2 SEQ1 SEQ0
 * ===================================================================== */
typedef struct {
  uint8_t seq : 3;   // 0..7
  uint8_t hb  : 1;   // heartbeat watchdog
  uint8_t c1  : 1;   // Startup
  uint8_t c2  : 1;   // N2O Fill
  uint8_t c3  : 1;   // Igniter Start
  uint8_t c4  : 1;   // Engine Start
} cmd_bits_t;

typedef union {
  cmd_bits_t bits;
  uint8_t    byte;
} cmd_byte_t;

static inline cmd_byte_t cmd_make(uint8_t seq, bool hb, bool c1, bool c2, bool c3, bool c4);

/* =====================================================================
 * SAS -> GSE (UART) framing
 *  Start = '$' (8 bits)
 *  End   = '$' (8 bits)
 *  Avec byte-stuffing pour éviter qu'un '$' apparaisse dans DATA/CRC.
 * ===================================================================== */
#define SAS_UART_FLAG     ((uint8_t)'$')   // 0x24 : début / fin de trame
#define SAS_UART_ESC      ((uint8_t)'\\')  // 0x5C : caractère d’échappement
#define SAS_UART_ESC_XOR  ((uint8_t)0x20)

/* =========================
 * CAN IDs (11-bit)
 * ========================= */
enum {
  CAN_ID_ESTOP        = 0x100,
  CAN_ID_HEARTBEAT    = 0x110,
  CAN_ID_STATUS_MIN   = 0x120,

  CAN_ID_CMD_MOTEUR   = 0x200,   // DLC=1, data[0] = cmd_byte_t.byte
  CAN_ID_RSP_SHORT    = 0x210,
  CAN_ID_DATA_SEG     = 0x300,
};
