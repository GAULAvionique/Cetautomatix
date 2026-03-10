#include "protocol_defs.h"

cmd_byte_t cmd_make(uint8_t seq, bool hb, bool c1, bool c2, bool c3, bool c4) {
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

motor_status_t motor_status_make(uint8_t seq,
                                 bool hb,
                                 bool spark_plug_detector,
                                 uint8_t battery_percent,
                                 uint8_t command_states,
                                 uint16_t n2o_pressure,
                                 uint8_t n2o_dump_valve,
                                 uint8_t n2o_igniter_valve,
                                 uint8_t n2o_main_valve,
                                 uint8_t ipa_igniter_valve,
                                 uint8_t ipa_main_valve,
                                 uint16_t timestamp_ms)
{
  motor_status_t st;

  st.seq                 = (uint8_t)(seq & 0x07u);
  st.hb                  = hb;
  st.spark_plug_detector = spark_plug_detector;
  st.battery_percent     = (uint8_t)(battery_percent & 0x07u);
  st.command_states      = (uint8_t)(command_states & 0x0Fu);
  st.n2o_pressure        = n2o_pressure;
  st.n2o_dump_valve      = (uint8_t)(n2o_dump_valve & 0x0Fu);
  st.n2o_igniter_valve   = (uint8_t)(n2o_igniter_valve & 0x0Fu);
  st.n2o_main_valve      = (uint8_t)(n2o_main_valve & 0x0Fu);
  st.ipa_igniter_valve   = (uint8_t)(ipa_igniter_valve & 0x0Fu);
  st.ipa_main_valve      = (uint8_t)(ipa_main_valve & 0x0Fu);
  st.timestamp_ms        = timestamp_ms;

  return st;
}

void motor_status_pack(const motor_status_t *st, uint8_t out[10])
{
  if (!st || !out) return;

  out[0] = TRAMES_FLAG;

  out[1] =
      (uint8_t)(((st->seq & 0x07u) << 5) |
                ((st->hb ? 1u : 0u) << 4) |
                ((st->spark_plug_detector ? 1u : 0u) << 3) |
                (st->battery_percent & 0x07u));

  out[2] =
      (uint8_t)(((st->command_states & 0x0Fu) << 4) |
                (st->n2o_dump_valve & 0x0Fu));

  out[3] = (uint8_t)((st->n2o_pressure >> 8) & 0xFFu);
  out[4] = (uint8_t)(st->n2o_pressure & 0xFFu);

  out[5] =
      (uint8_t)(((st->n2o_igniter_valve & 0x0Fu) << 4) |
                (st->n2o_main_valve & 0x0Fu));

  out[6] =
      (uint8_t)(((st->ipa_igniter_valve & 0x0Fu) << 4) |
                (st->ipa_main_valve & 0x0Fu));

  out[7] = (uint8_t)((st->timestamp_ms >> 8) & 0xFFu);
  out[8] = (uint8_t)(st->timestamp_ms & 0xFFu);

  out[9] = TRAMES_FLAG;
}

bool motor_status_unpack(motor_status_t *st, const uint8_t in[10])
{
  if (!st || !in) return false;

  if ((in[0] != TRAMES_FLAG) || (in[9] != TRAMES_FLAG)) {
    return false;
  }

  st->seq = (uint8_t)((in[1] >> 5) & 0x07u);
  st->hb  = (((in[1] >> 4) & 0x01u) != 0u);

  st->spark_plug_detector = (((in[1] >> 3) & 0x01u) != 0u);
  st->battery_percent     = (uint8_t)(in[1] & 0x07u);

  st->command_states = (uint8_t)((in[2] >> 4) & 0x0Fu);
  st->n2o_dump_valve = (uint8_t)(in[2] & 0x0Fu);

  st->n2o_pressure =
      (uint16_t)(((uint16_t)in[3] << 8) |
                 (uint16_t)in[4]);

  st->n2o_igniter_valve = (uint8_t)((in[5] >> 4) & 0x0Fu);
  st->n2o_main_valve    = (uint8_t)(in[5] & 0x0Fu);

  st->ipa_igniter_valve = (uint8_t)((in[6] >> 4) & 0x0Fu);
  st->ipa_main_valve    = (uint8_t)(in[6] & 0x0Fu);

  st->timestamp_ms =
      (uint16_t)(((uint16_t)in[7] << 8) |
                 (uint16_t)in[8]);

  return true;
}
