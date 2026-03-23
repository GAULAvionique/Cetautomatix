#include "GAUL/Drivers/control_valves.h"


int8_t ValveSystem_Init(valveSystem_t *dev) {
	if(!dev || !dev->ch1.port || !dev->ch2.port || !dev->servo.htim) return -1;

	dev->ch1.energize 		= false;
	dev->ch2.energize 		= false;
	dev->servo.open 		= false;
	dev->servo.last_state 	= false;
	dev->servo.is_moving    = false;
	dev->servo.start_time   = 0;
	dev->servo.timeout      = 0;

    HAL_GPIO_WritePin(dev->ch1.port, dev->ch1.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(dev->ch2.port, dev->ch2.pin, GPIO_PIN_RESET);

    HAL_TIM_PWM_Start(dev->servo.htim, dev->servo.channel);
    __HAL_TIM_SET_COMPARE(dev->servo.htim, dev->servo.channel, SERVO_PULSE_IDLE);

    return 0;
}

void Valve_ControlSolenoid(valveSystem_t *dev, valve_instance_t valve, bool energize) {
	if(!dev) return;

    GPIO_PinState pin_state = energize ? GPIO_PIN_SET : GPIO_PIN_RESET;

    if(valve == VALVE_CH1) {
    	dev->ch1.energize = energize;
        HAL_GPIO_WritePin(dev->ch1.port, dev->ch1.pin, pin_state);
    }
    else if(valve == VALVE_CH2) {
    	dev->ch2.energize = energize;
        HAL_GPIO_WritePin(dev->ch2.port, dev->ch2.pin, pin_state);
    } else return;
}

void Valve_ControlServo(valveSystem_t *dev, int8_t speed_percent, uint32_t timeout) {
	if(!dev || (speed_percent != 0 && timeout == 0)) return;

	if(dev->servo.is_moving) return;

    if(speed_percent > 100)  speed_percent = 100;
    if(speed_percent < -100) speed_percent = -100;

    // Securities
    if(!dev->servo.last_state && speed_percent < 0) return; // already closed
    if(dev->servo.last_state && speed_percent > 0) return;	// already open

    if(speed_percent == 0) {
		__HAL_TIM_SET_COMPARE(dev->servo.htim, dev->servo.channel, SERVO_PULSE_IDLE);
		dev->servo.is_moving = false;
		return;
	}

    uint32_t pulse_us = SERVO_PULSE_IDLE + (speed_percent * 10);
	__HAL_TIM_SET_COMPARE(dev->servo.htim, dev->servo.channel, pulse_us);

	dev->servo.start_time   = HAL_GetTick();
	dev->servo.timeout      = timeout;
	dev->servo.is_moving    = true;
	dev->servo.target_state = (speed_percent > 0) ? true : false;
}

bool ValveSystem_Update(valveSystem_t *dev) {
    if(!dev || !dev->servo.is_moving) return false;

    if(HAL_GetTick() - dev->servo.start_time >= dev->servo.timeout) {
        __HAL_TIM_SET_COMPARE(dev->servo.htim, dev->servo.channel, SERVO_PULSE_IDLE);
        dev->servo.is_moving = false;

        dev->servo.open = dev->servo.target_state;
        dev->servo.last_state = dev->servo.target_state;

        return true;
    }

    return false;
}
