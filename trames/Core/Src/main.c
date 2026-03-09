/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "protocol_defs.h"
#include "comm_utils.h"
#include "test_runner.h"
#include "comm_tx.h"
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */

  /* ---- CAN Filter: accept all ---- */
  CAN_Filter_Config();

  if (HAL_CAN_Start(&hcan1) != HAL_OK) {
    Error_Handler();
  }

  /* ===== Fin tests ===== */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
#ifdef UNIT_TEST
  run_all_tests();
  while (1) {
  }
#else
  /* ===== Tests UART4 & CAN1 ===== */
  const char *msg = "PROTO TEST: UART($..$) + CAN(DLC=1)\r\n";
  HAL_UART_Transmit(&huart4, (uint8_t*)msg, strlen(msg), 100);





  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    static uint8_t hb = 0;
	uint32_t t0;

	CAN_RxHeaderTypeDef  canRxHdr = {0};
	uint8_t can_rxd[8] = {0};

	hb ^= 1u;

	/* ---- Construire une commande "Startup" ---- */
	uint8_t seq = comm_seq_next();
	cmd_byte_t cmd = cmd_make(seq, hb,
							  true,   /* C1: Startup */
							  false,  /* C2: N2O Fill */
							  false,  /* C3: Igniter Start */
							  false); /* C4: Engine Start */

	/* 1) SAS->GSE via UART4 */
	if (comm_tx_rf_send_sas_cmd(&huart4, 50, cmd.byte) != HAL_OK) {
	  Error_Handler();
	}
	char buf[32];
	sprintf(buf, "CMD = 0x%02X\r\n", cmd.byte);
	HAL_UART_Transmit(&huart4, (uint8_t*)buf, strlen(buf), 100);

	/* 2) GSE->Moteur via CAN1 */
	if (comm_tx_can_send_moteur_cmd(&hcan1, cmd.byte) != HAL_OK) {
	  Error_Handler();
	}

	/* 3) Vérifier RX CAN en LOOPBACK */
	t0 = HAL_GetTick();
	while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0U) {
	  if ((HAL_GetTick() - t0) > 50U) {
		Error_Handler();   /* timeout RX */
	  }
	}

	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHdr, can_rxd) != HAL_OK) {
	  Error_Handler();
	}

	/* Vérifications attendues */
	if ((canRxHdr.DLC != 1U) || (can_rxd[0] != cmd.byte)) {
	  Error_Handler();
	}

	HAL_Delay(500);

	/* ---- Exemple : envoyer N2O Fill ---- */
	seq = comm_seq_next();
	cmd = cmd_make(seq, hb,
				   false,
				   true,
				   false,
				   false);

	if (comm_tx_rf_send_sas_cmd(&huart4, 50, cmd.byte) != HAL_OK) {
	  Error_Handler();
	}

	if (comm_tx_can_send_moteur_cmd(&hcan1, cmd.byte) != HAL_OK) {
	  Error_Handler();
	}

	t0 = HAL_GetTick();
	while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0U) {
	  if ((HAL_GetTick() - t0) > 50U) {
		Error_Handler();
	  }
	}

	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHdr, can_rxd) != HAL_OK) {
	  Error_Handler();
	}

	if ((canRxHdr.DLC != 1U) || (can_rxd[0] != cmd.byte)) {
	  Error_Handler();
	}

	HAL_Delay(500);

	/* ---- Exemple : E-STOP ---- */
	if (comm_tx_can_send_estop(&hcan1, 0x01) != HAL_OK) {
	  Error_Handler();
	}

	t0 = HAL_GetTick();
	while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0U) {
	  if ((HAL_GetTick() - t0) > 50U) {
		Error_Handler();
	  }
	}

	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHdr, can_rxd) != HAL_OK) {
	  Error_Handler();
	}

	/* Ici on vérifie juste DLC=1 et data=0x01 */
	if ((canRxHdr.DLC != 1U) || (can_rxd[0] != 0x01U)) {
	  Error_Handler();
	}

	HAL_Delay(1500);

  }
#endif
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
