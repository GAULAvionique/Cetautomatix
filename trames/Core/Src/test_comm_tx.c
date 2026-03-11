#include "comm_tx.h"
#include "protocol_defs.h"
#include <string.h>
#include <test_assert.h>
#include <test_comm_tx.h>

/* =========================
   Fakes pour observer les appels
   ========================= */

static int fake_free_level = 1;
static HAL_StatusTypeDef fake_can_add_status = HAL_OK;
static HAL_StatusTypeDef fake_uart_tx_status = HAL_OK;

static CAN_TxHeaderTypeDef captured_can_header;
static uint8_t captured_can_data[8];
static uint32_t captured_can_mb = 0;

static uint8_t captured_uart_data[8];
static uint16_t captured_uart_len = 0;
static uint32_t captured_uart_timeout = 0;


/* Redéfinition des wrappers */
#ifdef UNIT_TEST
int comm_hal_can_get_free_level(CAN_HandleTypeDef* hcan)
{
    (void)hcan;
    return fake_free_level;
}

HAL_StatusTypeDef comm_hal_can_add_tx(CAN_HandleTypeDef* hcan,
                                      CAN_TxHeaderTypeDef* tx,
                                      uint8_t* d,
                                      uint32_t* mb)
{
    (void)hcan;
    captured_can_header = *tx;
    memcpy(captured_can_data, d, 8);
    *mb = 1;
    captured_can_mb = *mb;
    return fake_can_add_status;
}

HAL_StatusTypeDef comm_hal_uart_transmit(UART_HandleTypeDef* huart,
                                         uint8_t* data,
                                         uint16_t len,
                                         uint32_t timeout)
{
    (void)huart;
    memset(captured_uart_data, 0, sizeof(captured_uart_data));
    memcpy(captured_uart_data, data, len);
    captured_uart_len = len;
    captured_uart_timeout = timeout;
    return fake_uart_tx_status;
}
#endif

static void reset_fakes(void)
{
    fake_free_level = 1;
    fake_can_add_status = HAL_OK;
    fake_uart_tx_status = HAL_OK;
    memset(&captured_can_header, 0, sizeof(captured_can_header));
    memset(captured_can_data, 0, sizeof(captured_can_data));
    captured_can_mb = 0;
    memset(captured_uart_data, 0, sizeof(captured_uart_data));
    captured_uart_len = 0;
    captured_uart_timeout = 0;
}

static void test_can_send_cmd_ok(void)
{
    reset_fakes();
    CAN_HandleTypeDef hcan;

    int ret = comm_tx_can_send_moteur_cmd(&hcan, 0xA5);

    TEST_ASSERT_EQ_U32(0, ret);
    TEST_ASSERT_EQ_U32((CAN_ID_CMD_MOTOR & 0x7FFu), captured_can_header.StdId);
    TEST_ASSERT_EQ_U32(1, captured_can_header.DLC);
    TEST_ASSERT_EQ_U8(0xA5, captured_can_data[0]);
}

static void test_can_send_cmd_no_mailbox(void)
{
    reset_fakes();
    CAN_HandleTypeDef hcan;
    fake_free_level = 0;

    int ret = comm_tx_can_send_moteur_cmd(&hcan, 0x55);

    TEST_ASSERT_EQ_U32((uint32_t)-2, (uint32_t)ret);
}

static void test_can_send_estop_ok(void)
{
    reset_fakes();
    CAN_HandleTypeDef hcan;

    int ret = comm_tx_can_send_estop(&hcan, 0x01);

    TEST_ASSERT_EQ_U32(0, ret);
    TEST_ASSERT_EQ_U32((CAN_ID_ESTOP & 0x7FFu), captured_can_header.StdId);
    TEST_ASSERT_EQ_U32(8, captured_can_header.DLC);
    TEST_ASSERT_EQ_U8('E', captured_can_data[0]);
    TEST_ASSERT_EQ_U8('S', captured_can_data[1]);
    TEST_ASSERT_EQ_U8('T', captured_can_data[2]);
    TEST_ASSERT_EQ_U8('P', captured_can_data[3]);
    TEST_ASSERT_EQ_U8(0x01, captured_can_data[4]);
}

static void test_uart_send_cmd_ok(void)
{
    reset_fakes();
    UART_HandleTypeDef huart;
    uint8_t cmd = 0x12;

    int ret = comm_tx_rf_send_gse_cmd(&huart, 50, cmd);

    TEST_ASSERT_EQ_U32(0, ret);
    TEST_ASSERT_TRUE(captured_uart_len >= 4);
    TEST_ASSERT_EQ_U8(TRAMES_FLAG, captured_uart_data[0]);
    TEST_ASSERT_EQ_U8(TRAMES_FLAG, captured_uart_data[captured_uart_len - 1]);
    TEST_ASSERT_EQ_U32(50, captured_uart_timeout);
}

void test_comm_tx_all(void)
{
    test_can_send_cmd_ok();
    test_can_send_cmd_no_mailbox();
    test_can_send_estop_ok();
    test_uart_send_cmd_ok();
}
