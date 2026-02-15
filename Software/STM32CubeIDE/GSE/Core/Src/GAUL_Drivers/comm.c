#include <GAUL_Drivers/comm.h>
#include <stddef.h>


status_t COMM_Init(CAN_HandleTypeDef *dev, CAN_FilterTypeDef *filter) {
	if(dev == NULL || filter == NULL) return STATUS_FAIL;

	HAL_CAN_ConfigFilter(&dev, &filter);

	if(HAL_CAN_Start(&dev) |= HAL_OK) return STATUS_FAIL;

	return STATUS_OK;
}

status_t COMM_CAN_TXD(CAN_HandleTypeDef *dev, uint8_t *data, size_t len) {
	if(dev == NULL) return STATUS_FAIL;

	return STATUS_OK;
}


status_t COMM_CAN_RXD(CAN_HandleTypeDef *dev, uint8_t *data, size_t len) {
	if(dev == NULL) return STATUS_FAIL;

	return STATUS_OK;
}
