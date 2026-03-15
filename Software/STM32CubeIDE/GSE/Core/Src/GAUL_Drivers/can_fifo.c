#include <GAUL_Drivers/can_fifo.h>


static CAN_RxCallback rx_callback = NULL;

/*----------------------------
 CAN Init
----------------------------*/
int8_t CAN_FIFO_Init(can_t *dev, CAN_HandleTypeDef *hcan) {
	if(!dev || !hcan) return -1;

    dev->hcan = hcan;

    if(HAL_CAN_Start(dev->hcan) != HAL_OK) return -1;

    HAL_CAN_ActivateNotification(dev->hcan,
        CAN_IT_RX_FIFO0_MSG_PENDING |
        CAN_IT_RX_FIFO1_MSG_PENDING |
        CAN_IT_BUSOFF |
        CAN_IT_ERROR_WARNING |
        CAN_IT_ERROR_PASSIVE |
        CAN_IT_ERROR
    );

    CAN_FIFO_SetTXHeader(dev, CAN_ID_NORMAL, 1);

    return 0;
}

/*----------------------------
 CAN TX with package priority
----------------------------*/
int8_t CAN_FIFO_Send(can_t *dev, const uint8_t *data) {
    if(!dev || !data) return -1;

    uint32_t txMailbox;
    if(HAL_CAN_AddTxMessage(dev->hcan, &dev->tx_header, (uint8_t *)data, &txMailbox) != HAL_OK)
        return -1;

    return 0;
}

void CAN_FIFO_SetTXHeader(can_t *dev, uint32_t id, uint8_t len) {
    if(!dev || len == 0) return;

    dev->tx_header.StdId = (id & 0x7FFu);
    dev->tx_header.ExtId = 0;
    dev->tx_header.IDE = CAN_ID_STD;
    dev->tx_header.RTR = CAN_RTR_DATA;
    dev->tx_header.DLC = len;
    dev->tx_header.TransmitGlobalTime = DISABLE;
}

/*----------------------------
 CAN RX0 Interrupt
----------------------------*/
void CAN_FIFO_RX0_IRQHandler_Callback(can_t *dev) {
    CAN_RxHeaderTypeDef rxHeader;
    uint8_t data[8];

    if(HAL_CAN_GetRxMessage(dev->hcan, CAN_RX_FIFO0, &rxHeader, data) == HAL_OK) {
        if(rx_callback) {
            rx_callback(rxHeader.StdId, data, rxHeader.DLC);
        }
    }
}

/*----------------------------
 CAN RX1 Interrupt
----------------------------*/
void CAN_FIFO_RX1_IRQHandler_Callback(can_t *dev) {
    CAN_RxHeaderTypeDef rxHeader;
    uint8_t data[8];

    if(HAL_CAN_GetRxMessage(dev->hcan, CAN_RX_FIFO1, &rxHeader, data) == HAL_OK) {
        if(rx_callback) {
            rx_callback(rxHeader.StdId, data, rxHeader.DLC);
        }
    }
}

/*----------------------------
 CAN SCE Interrupt
----------------------------*/
void CAN_FIFO_SCE_IRQHandler_Callback(can_t *dev) {
    uint32_t error = HAL_CAN_GetError(dev->hcan);
    if(error & HAL_CAN_ERROR_BOF) {
        HAL_CAN_ResetError(dev->hcan);
        HAL_CAN_Start(dev->hcan);
    }
}

/*----------------------------
 CAN Callback
----------------------------*/
void CAN_FIFO_SetRxCallback(can_t *dev, CAN_RxCallback callback) {
    rx_callback = callback;
}
