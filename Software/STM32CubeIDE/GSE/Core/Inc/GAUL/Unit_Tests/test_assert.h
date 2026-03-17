#pragma once
#include <stdint.h>

extern uint32_t g_test_pass;
extern uint32_t g_test_fail;

void test_log(const char *msg);
void test_log_hex_u32(const char *label, uint32_t value);

#define TEST_ASSERT_TRUE(expr)                                      \
    do {                                                            \
        if (expr) {                                                 \
            g_test_pass++;                                          \
            test_log("[PASS] " #expr "\r\n");                       \
        } else {                                                    \
            g_test_fail++;                                          \
            test_log("[FAIL] " #expr "\r\n");                       \
        }                                                           \
    } while (0)

#define TEST_ASSERT_EQ_U8(expected, actual)                         \
    do {                                                            \
        uint8_t exp__ = (uint8_t)(expected);                        \
        uint8_t act__ = (uint8_t)(actual);                          \
        if (exp__ == act__) {                                       \
            g_test_pass++;                                          \
            test_log("[PASS] " #actual "\r\n");                     \
        } else {                                                    \
            g_test_fail++;                                          \
            test_log("[FAIL] " #actual "\r\n");                     \
        }                                                           \
    } while (0)

#define TEST_ASSERT_EQ_U16(expected, actual)                        \
    do {                                                            \
        uint16_t exp__ = (uint16_t)(expected);                      \
        uint16_t act__ = (uint16_t)(actual);                        \
        if (exp__ == act__) {                                       \
            g_test_pass++;                                          \
            test_log("[PASS] " #actual "\r\n");                     \
        } else {                                                    \
            g_test_fail++;                                          \
            test_log("[FAIL] " #actual "\r\n");                     \
        }                                                           \
    } while (0)

#define TEST_ASSERT_EQ_U32(expected, actual)                        \
    do {                                                            \
        uint32_t exp__ = (uint32_t)(expected);                      \
        uint32_t act__ = (uint32_t)(actual);                        \
        if (exp__ == act__) {                                       \
            g_test_pass++;                                          \
            test_log("[PASS] " #actual "\r\n");                     \
        } else {                                                    \
            g_test_fail++;                                          \
            test_log("[FAIL] " #actual "\r\n");                     \
        }                                                           \
    } while (0)
