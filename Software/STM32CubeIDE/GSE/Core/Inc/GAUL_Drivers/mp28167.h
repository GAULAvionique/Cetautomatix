#pragma once

#include "stm32f1xx_hal.h"
#include <stdint.h>


#define MP28167_I2C_ADDRESS 0x60

#define	MP28167_VREF_REG	0x00


typedef enum {
	PFM,
	PWM
} mp28167_mode_t;

typedef struct {
	I2C_HandleTypeDef 	*hi2c;
	GPIO_TypeDef		*pg_power;
	uint16_t			pg_pin;
	uint16_t			vref;		/* 11 bits */
	uint8_t				vout;
	mp28167_mode_t		mode;
} mp28167_t;


void MP28167_Init(mp28167_t *dev, I2C_HandleTypeDef *hi2c, I2C_HandleTypeDef *pg_port, uint16_t pg_pin);

int8_t MP28167_SetVref(mp28167_t *dev, uint16_t vref);
int8_t MP28167_SetVout(mp28167_t *dev, uint16_t vout_target, float r1, float r2);
int8_t MP28167_SetMode(mp28167_t *dev, mp28167_mode_t mode);
