#pragma once

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>


#define MP28167_I2C_ADDR 		(0x60 << 1)

#define MP28167_REG_VREF_L      0x00
#define MP28167_REG_VREF_H      0x01
#define MP28167_REG_VREF_GO     0x02
#define MP28167_REG_IOUT_LIM    0x03
#define MP28167_REG_CTL1        0x04
#define MP28167_REG_CTL2        0x05
#define MP28167_REG_STATUS      0x09
#define MP28167_REG_INTERRUPT   0x0A
#define MP28167_REG_MASK        0x0B
#define MP28167_REG_DEV_ID      0x28

#define MP28167_VREF_MAX		1637.0f


typedef enum {
	PSM,		// PFM/PWM : reduce power comsumption
	PWM			// Forced PWM : reduce output noise
} mp28167_mode_t;

typedef struct {
	I2C_HandleTypeDef 	*hi2c;
	GPIO_TypeDef		*pg_power;
	uint16_t			pg_pin;
	uint16_t			vref;		/* 11 bits (mV) */
	uint8_t				vout;
	mp28167_mode_t		mode;
} mp28167_t;


int8_t MP28167_Init(mp28167_t *dev, I2C_HandleTypeDef *hi2c, GPIO_TypeDef *pg_port, uint16_t pg_pin);

int8_t MP28167_SetVref(mp28167_t *dev, float vref);
int8_t MP28167_SetVout(mp28167_t *dev, uint16_t vout_target, float r1, float r2);
int8_t MP28167_SetMode(mp28167_t *dev, mp28167_mode_t mode);
bool MP28167_IsPowerGood(mp28167_t *dev);
