#include "main.h"
#include <string.h>
#include <stdio.h>
#include <test_assert.h>
#include <test_comm_tx.h>
#include <test_comm_utils.h>
#include <test_runner.h>

extern UART_HandleTypeDef huart4;

uint32_t g_test_pass = 0;
uint32_t g_test_fail = 0;

void test_log(const char *msg)
{
    HAL_UART_Transmit(&huart4, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}

void test_log_hex_u32(const char *label, uint32_t value)
{
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "%s0x%08lX\r\n", label, value);
    if (n > 0) {
        HAL_UART_Transmit(&huart4, (uint8_t*)buf, (uint16_t)n, 100);
    }
}

void run_all_tests(void)
{
    test_log("\r\n===== START TESTS =====\r\n");

    test_comm_utils_all();
    test_comm_tx_all();

    test_log("\r\n===== TEST SUMMARY =====\r\n");
    test_log_hex_u32("PASS = ", g_test_pass);
    test_log_hex_u32("FAIL = ", g_test_fail);
}
