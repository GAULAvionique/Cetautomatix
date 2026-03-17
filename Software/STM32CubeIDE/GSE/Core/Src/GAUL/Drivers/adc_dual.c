#include "GAUL/Drivers/adc_dual.h"


#define ADC_VAL_TO_VOLTS(adc_val) 	(((float)(adc_val)) * (3.3f / 4095.0f))

static ring_buffer_t adc_ring;
static adc_dual_t ring_storage[ADC_DUAL_RING_SIZE];
static uint32_t adc_dual_dma_buffer[ADC_DUAL_DMA_TRANSFERT_SIZE]; // 4 x 32 bits (dma dual word)


int8_t ADC_DUAL_Init(adc_dual_t *dev, ADC_HandleTypeDef* hadc1, ADC_HandleTypeDef* hadc2, TIM_HandleTypeDef* htim_trigger) {
	if(!dev || !hadc1 || !hadc2 || !htim_trigger) return -1;

	dev->hadc1 = hadc1;
	dev->hadc2 = hadc2;
	dev->htim_trigger = htim_trigger;
	dev->load_cell_offset = 0.0f;
	dev->pression_offset = 0.0f;

	RingBuffer_Init(&adc_ring, ring_storage, ADC_DUAL_RING_SIZE, sizeof(adc_raw_frame_t));

	HAL_ADCEx_Calibration_Start(dev->hadc1);
	HAL_ADCEx_Calibration_Start(dev->hadc2);

	HAL_ADC_Start(dev->hadc2);
	HAL_ADCEx_MultiModeStart_DMA(dev->hadc1, adc_dual_dma_buffer, ADC_DUAL_DMA_TRANSFERT_SIZE);

	HAL_TIM_Base_Start(dev->htim_trigger);

	return 0;
}

uint16_t ADC_DUAL_Available(adc_dual_t *dev) {
	return RingBuffer_Available(&adc_ring);
}

bool ADC_DUAL_ReadFrame(adc_dual_t *dev) {
	if(!dev) return false;

	if(ADC_DUAL_Available(dev) >= 1) {
		adc_raw_frame_t raw_data;
		RingBuffer_Read(&adc_ring, &raw_data, 1);

		float v_an;
		// Temp
		v_an = ADC_VAL_TO_VOLTS(raw_data.adc1_channels[0]);
		dev->temperature = (v_an - 0.40f) / 0.01953f;
		// V12
		v_an = ADC_VAL_TO_VOLTS(raw_data.adc1_channels[1]);
		dev->v12 = v_an * ADC_VOLT_TO_V12;
		// V5
		v_an = ADC_VAL_TO_VOLTS(raw_data.adc1_channels[2]);
		dev->v5 = v_an * ADC_VOLT_TO_V5;
		// V3
		v_an = ADC_VAL_TO_VOLTS(raw_data.adc1_channels[3]);
		dev->v3 = v_an * ADC_VOLT_TO_V3;
		// Vin
		v_an = ADC_VAL_TO_VOLTS(raw_data.adc2_channels[0]);
		dev->vin = v_an * ADC_VOLT_TO_VIN;
		// Load Cell
		v_an = ADC_VAL_TO_VOLTS(raw_data.adc2_channels[1]);
		float tension_diff_v = v_an / ADC_VOLT_TO_LCELL_GAIN;
		float absolute_weight = (tension_diff_v / LCELL_MAX_VDIFF) * LCELL_GSE_MAX_KG;
		dev->load_cell = absolute_weight - dev->load_cell_offset;
		// Pression
		v_an = ADC_VAL_TO_VOLTS(raw_data.adc2_channels[2]);
		float pression_current = v_an / ADC_VOLT_TO_CURRENT_PRESSION;
		// (I - I_min) / (I_max - I_min) * pression_max - offset
		float absolute_pression = ((pression_current - 0.004f) / 0.016f) * PRESSION_MAX_PSI;
		dev->pression = absolute_pression - dev->pression_offset;

		return true;
	}

	return false;
}

float ADC_DUAL_TareLCell(adc_dual_t *dev) {
    HAL_Delay(100);
    while(ADC_DUAL_ReadFrame(dev)) {}	// wipe buffer
    while(!ADC_DUAL_ReadFrame(dev)) {}	// wait until new measures

    dev->load_cell_offset += dev->load_cell;

    return dev->load_cell_offset;
}

float ADC_DUAL_TarePression(adc_dual_t *dev) {
    HAL_Delay(100);
    while(ADC_DUAL_ReadFrame(dev)) {}	// wipe buffer
    while(!ADC_DUAL_ReadFrame(dev)) {}	// wait until new measures

    dev->pression_offset += dev->pression;

    return dev->pression_offset;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if(hadc->Instance == ADC1) {
		adc_raw_frame_t current_reading;
		for(int i = 0; i < 4; i++) {
			current_reading.adc1_channels[i] = (uint16_t)(adc_dual_dma_buffer[i] & 0xFFFFu);

			if(i < 3) {
				current_reading.adc2_channels[i] = (uint16_t)((adc_dual_dma_buffer[i] >> 16) & 0xFFFFu);
			}
		}
		RingBuffer_Write(&adc_ring, &current_reading, 1);
	}
}
