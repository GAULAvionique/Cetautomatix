#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "can.h"
#include "usart.h"
#include "protocol_defs.h"

uint8_t comm_crc8_atm(const void* data, uint16_t len);

uint16_t comm_uart_build_cmd_frame(uint8_t cmd_byte, uint8_t* out, uint16_t out_max);
uint16_t comm_uart_build_motor_status_frame(const uint8_t raw_frame[10], uint8_t* out, uint16_t out_max);

// Sequence (modulo 8)
uint8_t comm_seq_next(void);

void dbg_puts(UART_HandleTypeDef *huart, const char *s);
void dbg_printf(UART_HandleTypeDef *huart, const char *fmt, ...);
bool wait_can_rx(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *rxh, uint8_t *data, uint32_t timeout_ms);
bool uart_unstuff_between_flags(const uint8_t *frame, uint16_t frame_len, uint8_t *decoded, uint16_t *decoded_len);
bool parse_uart_cmd_frame(const uint8_t *frame, uint16_t frame_len, uint8_t *cmd_byte_out);
bool parse_uart_motor_status_frame(const uint8_t *frame, uint16_t frame_len, motor_status_t *st_out);
uint8_t cmd_states_from_cmd_byte(uint8_t cmd_byte);
int recv_motor_status_from_can(CAN_HandleTypeDef *hcan, motor_status_t *st, uint32_t timeout_ms);

void run_end_to_end_test_once(CAN_HandleTypeDef* hcan,
                              UART_HandleTypeDef* huart,
                              bool *hb,
                              uint32_t timeout_ms);