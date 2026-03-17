#pragma once

#include <GAUL/Low_Level/ring_buffer.h>
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>


#define ADC_DUAL_RING_SIZE 				20 // 20 cycles (2s at 10Hz)
#define ADC_DUAL_DMA_TRANSFERT_SIZE 	4

#define ADC_VOLT_TO_V12					4.9f
#define ADC_VOLT_TO_V5					2.0f
#define ADC_VOLT_TO_V3					1.12f
#define ADC_VOLT_TO_VIN					9.3333f
#define ADC_VOLT_TO_LCELL_GAIN			304.0f
#define ADC_VOLT_TO_CURRENT_PRESSION	99.1694f

#define LCELL_GSE_MAX_KG                1000.0f // Max gse capacity (kg)
#define LCELL_MOTOR_MAX_KG              100.0f  // Max motor capacity (kg)
#define LCELL_MAX_VDIFF             	0.011f  // Max diff voltage (11mV = 5V * 2.2mV/V) (mV)
#define PRESSION_MAX_PSI            	1500.0f // Max capacity (psi)

typedef struct {
	uint16_t adc1_channels[4];
	uint16_t adc2_channels[3];
} adc_raw_frame_t;

typedef struct {
	ADC_HandleTypeDef	*hadc1;
	ADC_HandleTypeDef	*hadc2;
	TIM_HandleTypeDef	*htim_trigger;

	float               load_cell_offset;
	float				pression_offset;

	float				load_cell;
	float				pression;
	float				temperature;
	float 				vin;
	float				v12;
	float 				v5;
	float 				v3;
} adc_dual_t;


int8_t ADC_DUAL_Init(adc_dual_t *dev, ADC_HandleTypeDef* hadc1, ADC_HandleTypeDef* hadc2, TIM_HandleTypeDef* htim_trigger);

bool ADC_DUAL_ReadFrame(adc_dual_t *dev);
uint16_t ADC_DUAL_Available(adc_dual_t *dev);
float ADC_DUAL_TareLCell(adc_dual_t *dev);
float ADC_DUAL_TarePression(adc_dual_t *dev);
