/*
 * ltste682krkgwt.h
 *
 *  Created on: 3 mars 2026
 *      Author: AudaceLol12
 */

#pragma once

#include "stm32f1xx_hal.h"


typedef enum {
	GREEN,
	RED,
	ORANGE,
	NONE
} criticalLed_color_t;

void CriticalLED_SetColor(GPIO_TypeDef *port, uint16_t r_pin, uint16_t g_pin, criticalLed_color_t color);
