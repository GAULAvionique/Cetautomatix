#include "protocol_defs.h"

static inline cmd_byte_t cmd_make(uint8_t seq, bool hb, bool c1, bool c2, bool c3, bool c4) {
  cmd_byte_t c;
  c.byte = 0;
  c.bits.seq = (uint8_t)(seq & 0x7u);
  c.bits.hb  = hb ? 1u : 0u;
  c.bits.c1  = c1 ? 1u : 0u;
  c.bits.c2  = c2 ? 1u : 0u;
  c.bits.c3  = c3 ? 1u : 0u;
  c.bits.c4  = c4 ? 1u : 0u;
  return c;
}
