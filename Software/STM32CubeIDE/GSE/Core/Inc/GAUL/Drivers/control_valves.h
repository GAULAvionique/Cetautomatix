#pragma once

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define SERVO_PULSE_MIN    	500		// anti-clockwise max speed
#define SERVO_PULSE_IDLE 	1500	// stop
#define SERVO_PULSE_MAX    	2500  	// clockwise max speed

typedef enum {
	VALVE_CH1,
	VALVE_CH2,
	SERVO
} valve_instance_t;

typedef struct {
	TIM_HandleTypeDef 	*htim;
	uint32_t 			channel;
	bool 				open;			// 0 = close, 1 = open
	bool				last_state;		// open or not (only base with init -> close initially)

	bool                is_moving;
	uint32_t            start_time;
	uint32_t            timeout;
	bool                target_state;
} valve_servo_t;

typedef struct {
	GPIO_TypeDef 		*port;
	uint16_t			pin;
	bool 				energize;		// 0 = close (normally), 1 = open
} valve_solenoid_t;

typedef struct {
    valve_solenoid_t    ch1;
    valve_solenoid_t    ch2;
    valve_servo_t       servo;
} valveSystem_t;


int8_t ValveSystem_Init(valveSystem_t *dev);

void ValveSystem_ControlSolenoid(valveSystem_t *dev, valve_instance_t valve, bool energize);
void ValveSystem_ControlServo(valveSystem_t *dev, int8_t speed, uint32_t timeout);
bool ValveSystem_Update(valveSystem_t *dev);
