#pragma once
#include <stdint.h>

typedef enum {
  NODE_STATION = 0x01,
  NODE_GSE     = 0x02,
  NODE_ENGINE  = 0x03,
} node_id_t;

enum { PROTO_VER = 0x02 };  // ← bump version, protocole incompatible avec v1

typedef enum {
  OP_PING            = 0x01,
  OP_HEARTBEAT       = 0x02,

  OP_ARM             = 0x10,
  OP_DISARM          = 0x11,
  OP_ESTOP           = 0x12,

  OP_SET_VALVE       = 0x20,

  // --- Actions à durée/état -> opcodes START/STOP distincts ---
  OP_PURGE_START     = 0x22,
  OP_PURGE_STOP      = 0x24,

  OP_FILL_START      = 0x23,
  OP_FILL_STOP       = 0x25,

  OP_IGNITION_START  = 0x26,
  OP_IGNITION_ABORT  = 0x27,

  OP_READ_STATUS     = 0x30,
  OP_STATUS          = 0x31,
  OP_ERROR           = 0x40,
  OP_NACK            = 0x7F,
} opcode_t;


enum {
  CAN_ID_ESTOP      = 0x100,
  CAN_ID_HEARTBEAT  = 0x110,
  CAN_ID_STATUS_MIN = 0x120,
  CAN_ID_CMD_SHORT  = 0x200,
  CAN_ID_RSP_SHORT  = 0x210,
  CAN_ID_DATA_SEG   = 0x300,
};
