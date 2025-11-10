#pragma once
#include <stdint.h>

// CRC-16/CCITT-FALSE (poly 0x1021, init 0xFFFF)
uint16_t comm_crc16_ccitt(const void* data, uint16_t len);

// COBS encode: in[0..len-1] -> out (taille >= len + len/254 + 1); retourne taille encodée
uint16_t comm_cobs_encode(const uint8_t* in, uint16_t len, uint8_t* out);

// Sequence (1 octet modulo 256)
static inline uint8_t comm_seq_next(uint8_t cur) { return (uint8_t)(cur + 1); }
