#include "comm_tx_rf.h"

// tailles max (à adapter)
#define RF_MAX_PAYLOAD   128
#define RF_HDR_LEN       (1+1+1+1+1+4+2) // ver,src,dst,op,seq,ts,len
#define RF_BUF_RAW_MAX   (RF_HDR_LEN + RF_MAX_PAYLOAD + 2 /*crc*/)
#define RF_BUF_ENC_MAX   (RF_BUF_RAW_MAX + RF_BUF_RAW_MAX/254 + 1 /*COBS*/ + 1 /*delim*/)

int comm_tx_rf_send(UART_HandleTypeDef* huart, uint32_t timeout,
                    node_id_t src, node_id_t dst, uint8_t op,
                    uint8_t seq, uint32_t ts_ms,
                    const uint8_t* payload, uint16_t len)
{
  if (len > RF_MAX_PAYLOAD) return -2;

  uint8_t raw[RF_BUF_RAW_MAX];
  uint16_t w = 0;

  raw[w++] = PROTO_VER;
  raw[w++] = (uint8_t)src;   // <-- node_id_t vers octet
  raw[w++] = (uint8_t)dst;   // <-- node_id_t vers octet
  raw[w++] = op;
  raw[w++] = seq;

  raw[w++] = (uint8_t)(ts_ms & 0xFF);
  raw[w++] = (uint8_t)((ts_ms >> 8) & 0xFF);
  raw[w++] = (uint8_t)((ts_ms >> 16) & 0xFF);
  raw[w++] = (uint8_t)((ts_ms >> 24) & 0xFF);

  raw[w++] = (uint8_t)(len & 0xFF);
  raw[w++] = (uint8_t)(len >> 8);

  if (len && payload) {
    for (uint16_t i=0;i<len;i++) raw[w++] = payload[i];
  }

  uint16_t crc = comm_crc16_ccitt(raw, w);
  raw[w++] = (uint8_t)(crc & 0xFF);
  raw[w++] = (uint8_t)(crc >> 8);

  uint8_t enc[RF_BUF_ENC_MAX];
  uint16_t enc_len = comm_cobs_encode(raw, w, enc);
  enc[enc_len++] = 0x00;

  return (HAL_UART_Transmit(huart, enc, enc_len, timeout) == HAL_OK) ? 0 : -1;
}

