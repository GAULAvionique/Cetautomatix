#include <test_assert.h>
#include <test_comm_utils.h>
#include "comm_utils.h"
#include "protocol_defs.h"

static void test_seq_wrap(void)
{
    uint8_t a = comm_seq_next();
    uint8_t b = comm_seq_next();
    uint8_t c = comm_seq_next();
    uint8_t d = comm_seq_next();
    uint8_t e = comm_seq_next();
    uint8_t f = comm_seq_next();
    uint8_t g = comm_seq_next();
    uint8_t h = comm_seq_next();
    uint8_t i = comm_seq_next();

    TEST_ASSERT_EQ_U8(0, a);
    TEST_ASSERT_EQ_U8(1, b);
    TEST_ASSERT_EQ_U8(2, c);
    TEST_ASSERT_EQ_U8(3, d);
    TEST_ASSERT_EQ_U8(4, e);
    TEST_ASSERT_EQ_U8(5, f);
    TEST_ASSERT_EQ_U8(6, g);
    TEST_ASSERT_EQ_U8(7, h);
    TEST_ASSERT_EQ_U8(0, i);
}

static void test_crc8_empty(void)
{
    uint8_t crc = comm_crc8_atm(0, 0);
    TEST_ASSERT_EQ_U8(0x00, crc);
}

static void test_crc8_one_byte_repeatable(void)
{
    uint8_t x = 0x12;
    uint8_t a = comm_crc8_atm(&x, 1);
    uint8_t b = comm_crc8_atm(&x, 1);
    TEST_ASSERT_EQ_U8(a, b);
}

static void test_uart_frame_basic(void)
{
    uint8_t out[8] = {0};
    uint8_t cmd = 0x12;

    uint16_t n = comm_uart_build_cmd_frame(cmd, out, (uint16_t)sizeof(out));

    TEST_ASSERT_TRUE(n >= 4);
    TEST_ASSERT_EQ_U8(TRAMES_FLAG, out[0]);
    TEST_ASSERT_EQ_U8(cmd, out[1]);
    TEST_ASSERT_EQ_U8(comm_crc8_atm(&cmd, 1), out[2]);
    TEST_ASSERT_EQ_U8(TRAMES_FLAG, out[n - 1]);
}

static void test_uart_frame_escape_flag(void)
{
    uint8_t out[8] = {0};
    uint8_t cmd = TRAMES_FLAG;

    uint16_t n = comm_uart_build_cmd_frame(cmd, out, (uint16_t)sizeof(out));

    TEST_ASSERT_TRUE(n >= 5);
    TEST_ASSERT_EQ_U8(TRAMES_FLAG, out[0]);
    TEST_ASSERT_EQ_U8(SAS_UART_ESC, out[1]);
    TEST_ASSERT_EQ_U8((uint8_t)(cmd ^ SAS_UART_ESC_XOR), out[2]);
    TEST_ASSERT_EQ_U8(TRAMES_FLAG, out[n - 1]);
}

static void test_uart_frame_null_buffer(void)
{
    uint16_t n = comm_uart_build_cmd_frame(0x12, 0, 8);
    TEST_ASSERT_EQ_U16(0, n);
}

static void test_uart_frame_small_buffer(void)
{
    uint8_t out[3] = {0};
    uint16_t n = comm_uart_build_cmd_frame(0x12, out, 3);
    TEST_ASSERT_EQ_U16(0, n);
}

void test_comm_utils_all(void)
{
    test_seq_wrap();
    test_crc8_empty();
    test_crc8_one_byte_repeatable();
    test_crc16_empty();
    test_uart_frame_basic();
    test_uart_frame_escape_flag();
    test_uart_frame_null_buffer();
    test_uart_frame_small_buffer();
}
