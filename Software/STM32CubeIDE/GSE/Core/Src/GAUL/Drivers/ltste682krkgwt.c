/*
 * ltste682krkgwt.c
 *
 *  Created on: 3 mars 2026
 *      Author: AudaceLol12
 */


#include "GAUL/Drivers/ltste682krkgwt.h"


void CriticalLED_SetColor(GPIO_TypeDef *port, uint16_t r_pin, uint16_t g_pin, criticalLed_color_t color) {
	switch(color) {
		case GREEN:
			HAL_GPIO_WritePin(port, g_pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(port, r_pin, GPIO_PIN_SET);
			break;
		case RED:
			HAL_GPIO_WritePin(port, g_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(port, r_pin, GPIO_PIN_RESET);
			break;
		case ORANGE:
			HAL_GPIO_WritePin(port, g_pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(port, r_pin, GPIO_PIN_RESET);
			break;
		case NONE:
			HAL_GPIO_WritePin(port, g_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(port, r_pin, GPIO_PIN_SET);
			break;
		default :
			break;
	}
}
