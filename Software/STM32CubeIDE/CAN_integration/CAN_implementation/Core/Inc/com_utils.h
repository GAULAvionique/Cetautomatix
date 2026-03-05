/*
 * com_logic.h
 *
 *  Created on: 4 Mar 2026
 *      Author: Samuel
 */

#ifndef INC_COM_UTILS_H_
#define INC_COM_UTILS_H_

#include "../../TAGCAN/TAGCAN.h"

#include <cmsis_gcc.h>
#include <stm32f103xb.h>
#include <stm32f1xx.h>
#include <stm32f1xx_hal_can.h>
#include <stm32f1xx_hal_def.h>
#include <stm32f1xx_hal_flash.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_hal_rcc_ex.h>
#include <sys/_stdint.h>

extern uint8_t TxData[8];
extern uint8_t RxData[8];
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern CAN_HandleTypeDef hcan;
extern int32_t TxMailbox;
void init_com(void);


#endif /* INC_COM_UTILS_H_ */
