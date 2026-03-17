#include "GAUL_Drivers/mp28167.h"


static int8_t MP28167_ReadReg(mp28167_t *dev, uint8_t reg, uint8_t *data) {
    if(HAL_I2C_Mem_Read(dev->hi2c, MP28167_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY) == HAL_OK) {
        return 0;
    }

    return -1;
}

static int8_t MP28167_WriteReg(mp28167_t *dev, uint8_t reg, uint8_t data) {
    if(HAL_I2C_Mem_Write(dev->hi2c, MP28167_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY) == HAL_OK) {
        return 0;
    }

    return -1;
}


int8_t MP28167_Init(mp28167_t *dev, I2C_HandleTypeDef *hi2c, GPIO_TypeDef *pg_port, uint16_t pg_pin) {
	if(!dev || !hi2c || !pg_port) return -1;

    dev->hi2c 		= hi2c;
    dev->pg_power 	= pg_port;
    dev->pg_pin 	= pg_pin;
    dev->vref 		= 540;
    dev->vout 		= 0;
    dev->mode 		= PWM;

    return 0;
}

int8_t MP28167_SetVref(mp28167_t *dev, float vref) {
	if(!dev) return -1;

    if(vref < 0.0f) vref = 0.0f;
    if(vref > MP28167_VREF_MAX) vref = MP28167_VREF_MAX;

    uint16_t v_val = (uint16_t)(vref / 0.8f);
    uint8_t vref_l = (uint8_t)(v_val & 0x07);
    uint8_t vref_h = (uint8_t)((v_val >> 3) & 0xFF);

    if(MP28167_WriteReg(dev, MP28167_REG_VREF_L, vref_l) != 0) return -1;
    if(MP28167_WriteReg(dev, MP28167_REG_VREF_H, vref_h) != 0) return -1;

    uint8_t go_reg; // VREF_GO
    if(MP28167_ReadReg(dev, MP28167_REG_VREF_GO, &go_reg) != 0) return -1;
    go_reg |= 0x01;
    if(MP28167_WriteReg(dev, MP28167_REG_VREF_GO, go_reg) != 0) return -1;

    dev->vref = (uint16_t)vref;

    return 0;
}

int8_t MP28167_SetVout(mp28167_t *dev, uint16_t vout_target, float r1, float r2) {
	if(!dev || vout_target == 0 || r1 < 1 || r2 < 1) return -1;

    float vref_target_mv = (vout_target * 1000.0f) * (r2 / (r1 + r2));

    if(MP28167_SetVref(dev, vref_target_mv) != 0) return -1;

	dev->vout = (uint8_t)vout_target;

	return 0;
}

int8_t MP28167_SetMode(mp28167_t *dev, mp28167_mode_t mode) {
	if(!dev) return -1;

    uint8_t ctl1;
    if(MP28167_ReadReg(dev, MP28167_REG_CTL1, &ctl1) != 0) return -1;

    if(mode == PWM) {
        ctl1 |= (1 << 4);
    } else {
        ctl1 &= ~(1 << 4);
    }

    if(MP28167_WriteReg(dev, MP28167_REG_CTL1, ctl1) != 0) return -1;

	dev->mode = mode;

	return 0;
}

bool MP28167_IsPowerGood(mp28167_t *dev) {
    if(dev->pg_power != NULL) {
    	// Active LOW
        return (HAL_GPIO_ReadPin(dev->pg_power, dev->pg_pin) != GPIO_PIN_SET);
    }

    // Only if issue (maybe not needed)
    uint8_t status_reg = 0;
    if(MP28167_ReadReg(dev, MP28167_REG_STATUS, &status_reg) == 0) {
        return (status_reg & (1 << 7)) != 0;
    }

    return false;
}
