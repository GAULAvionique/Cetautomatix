/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ISO_CAN_IN_Pin GPIO_PIN_13
#define ISO_CAN_IN_GPIO_Port GPIOC
#define Pressure_Sleep_Pin GPIO_PIN_0
#define Pressure_Sleep_GPIO_Port GPIOD
#define LCell_Sleep_Pin GPIO_PIN_1
#define LCell_Sleep_GPIO_Port GPIOD
#define Pressure_An_Pin GPIO_PIN_0
#define Pressure_An_GPIO_Port GPIOA
#define LCell_An_Pin GPIO_PIN_1
#define LCell_An_GPIO_Port GPIOA
#define Temp_An_Pin GPIO_PIN_2
#define Temp_An_GPIO_Port GPIOA
#define V5_An_Pin GPIO_PIN_3
#define V5_An_GPIO_Port GPIOA
#define V3_An_Pin GPIO_PIN_4
#define V3_An_GPIO_Port GPIOA
#define Vin_An_Pin GPIO_PIN_5
#define Vin_An_GPIO_Port GPIOA
#define Power_Good_Pin GPIO_PIN_6
#define Power_Good_GPIO_Port GPIOA
#define CriticalLED_G_Pin GPIO_PIN_10
#define CriticalLED_G_GPIO_Port GPIOB
#define CriticalLED_R_Pin GPIO_PIN_11
#define CriticalLED_R_GPIO_Port GPIOB
#define Valve3_Open_Pin GPIO_PIN_10
#define Valve3_Open_GPIO_Port GPIOA
#define Valve2_Open_Pin GPIO_PIN_11
#define Valve2_Open_GPIO_Port GPIOA
#define Valve1_Open_Pin GPIO_PIN_12
#define Valve1_Open_GPIO_Port GPIOA
#define SYS_SWDIO_Pin GPIO_PIN_13
#define SYS_SWDIO_GPIO_Port GPIOA
#define SYS_SWCLK_Pin GPIO_PIN_14
#define SYS_SWCLK_GPIO_Port GPIOA
#define SYS_SWO_Pin GPIO_PIN_3
#define SYS_SWO_GPIO_Port GPIOB
#define RFD_USART1_TX_Pin GPIO_PIN_6
#define RFD_USART1_TX_GPIO_Port GPIOB
#define RFD_USART1_RX_Pin GPIO_PIN_7
#define RFD_USART1_RX_GPIO_Port GPIOB
#define ISO_CAN_RXD_Pin GPIO_PIN_8
#define ISO_CAN_RXD_GPIO_Port GPIOB
#define ISO_CAN_TXD_Pin GPIO_PIN_9
#define ISO_CAN_TXD_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
