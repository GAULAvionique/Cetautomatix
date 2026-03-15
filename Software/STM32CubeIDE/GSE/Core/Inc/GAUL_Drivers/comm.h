#pragma once

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include "GAUL_Drivers/frame_defs.h"
#include "GAUL_Drivers/rfd900x.h"
#include "GAUL_Drivers/can_fifo.h"


typedef struct {
	rfd900x_t				*rfd_dev;
	can_t					*can_dev;
	bool					isVID;
	uint8_t					seq;			// Exit sequence
	uint8_t					last_sas_seq;	// Last sequence received by SAS
	uint8_t					last_motor_seq;	// Last sequence received by MOTOR
} comm_gse_t;


/*----------------------------
 Init COMM GSE
----------------------------*/
int8_t COMM_Init(comm_gse_t *dev);

/*----------------------------
 Receive GSE <- SAS
----------------------------*/
int8_t COMM_ReceiveGSEFromSAS(comm_gse_t *dev, cmd_t *cmd);

/*----------------------------
 Transmit GSE -> MOTOR
----------------------------*/
int8_t COMM_TransmitGSEToMotor(comm_gse_t *dev, const cmd_t *cmd);

/*----------------------------
 Transmit GSE -> SAS
----------------------------*/
int8_t COMM_TransmitGSEToSAS(comm_gse_t *dev, const gse_state_frame_t *status, uint32_t timeout);
