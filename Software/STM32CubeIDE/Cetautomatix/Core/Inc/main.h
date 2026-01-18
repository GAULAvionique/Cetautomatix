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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EXTEND_IO2_Pin GPIO_PIN_13
#define EXTEND_IO2_GPIO_Port GPIOC
#define EXTEND_IO3_Pin GPIO_PIN_14
#define EXTEND_IO3_GPIO_Port GPIOC
#define EXTEND_IO4_Pin GPIO_PIN_15
#define EXTEND_IO4_GPIO_Port GPIOC
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
#define Sense_2_Pin GPIO_PIN_3
#define Sense_2_GPIO_Port GPIOA
#define Sense_1_Pin GPIO_PIN_4
#define Sense_1_GPIO_Port GPIOA
#define PA_An_Pin GPIO_PIN_5
#define PA_An_GPIO_Port GPIOA
#define V3_An_Pin GPIO_PIN_6
#define V3_An_GPIO_Port GPIOA
#define Vin_An_Pin GPIO_PIN_7
#define Vin_An_GPIO_Port GPIOA
#define Fire_2_Pin GPIO_PIN_0
#define Fire_2_GPIO_Port GPIOB
#define Fire_1_Pin GPIO_PIN_1
#define Fire_1_GPIO_Port GPIOB
#define Pyros_Arm_Pin GPIO_PIN_2
#define Pyros_Arm_GPIO_Port GPIOB
#define CriticalLED_R_Pin GPIO_PIN_10
#define CriticalLED_R_GPIO_Port GPIOB
#define CriticalLED_G_Pin GPIO_PIN_11
#define CriticalLED_G_GPIO_Port GPIOB
#define Servo4_Close_Pin GPIO_PIN_12
#define Servo4_Close_GPIO_Port GPIOB
#define Servo4_Open_Pin GPIO_PIN_13
#define Servo4_Open_GPIO_Port GPIOB
#define Servo3_Close_Pin GPIO_PIN_14
#define Servo3_Close_GPIO_Port GPIOB
#define Servo3_Open_Pin GPIO_PIN_15
#define Servo3_Open_GPIO_Port GPIOB
#define Servo4_PWM_Pin GPIO_PIN_8
#define Servo4_PWM_GPIO_Port GPIOA
#define Servo3_PWM_Pin GPIO_PIN_9
#define Servo3_PWM_GPIO_Port GPIOA
#define Servo2_PWM_Pin GPIO_PIN_10
#define Servo2_PWM_GPIO_Port GPIOA
#define Servo1_PWM_Pin GPIO_PIN_11
#define Servo1_PWM_GPIO_Port GPIOA
#define Servo1_Open_Pin GPIO_PIN_12
#define Servo1_Open_GPIO_Port GPIOA
#define SYS_SWDIO_Pin GPIO_PIN_13
#define SYS_SWDIO_GPIO_Port GPIOA
#define SYS_SWCLK_Pin GPIO_PIN_14
#define SYS_SWCLK_GPIO_Port GPIOA
#define Servo1_Close_Pin GPIO_PIN_15
#define Servo1_Close_GPIO_Port GPIOA
#define SYS_SWO_Pin GPIO_PIN_3
#define SYS_SWO_GPIO_Port GPIOB
#define Servo2_Open_Pin GPIO_PIN_4
#define Servo2_Open_GPIO_Port GPIOB
#define Servo2_Close_Pin GPIO_PIN_5
#define Servo2_Close_GPIO_Port GPIOB
#define EXTEND_IO1_Pin GPIO_PIN_6
#define EXTEND_IO1_GPIO_Port GPIOB
#define ISO_CAN_IN_Pin GPIO_PIN_7
#define ISO_CAN_IN_GPIO_Port GPIOB
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
