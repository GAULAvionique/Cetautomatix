#pragma once
#include <stdint.h>

// CRC-16/CCITT-FALSE (poly 0x1021, init 0xFFFF)
uint16_t comm_crc16_ccitt(const void* data, uint16_t len);

uint8_t comm_crc8_atm(const void* data, uint16_t len);

static void uart_push_escaped_byte(uint8_t byte, uint8_t* out, uint16_t* idx, uint16_t out_max);

uint16_t comm_uart_build_sas_frame(uint8_t cmd_byte, uint8_t* out, uint16_t out_max);

// Sequence (modulo 8)
uint8_t comm_seq_next(void);
