#pragma once

#include <stdint.h>
#include <stdbool.h>


/* =========================
 * Protocol Configuration
 * ========================= */
#define PROTO_VER 0x01

#if PROTO_VER != 0x01
#error "Unsupported protocol version"
#endif

/* =========================
 * Protocol Configuration
 * ========================= */
#define FRAME_UART_FLAG		((uint8_t)'$')  					// 0x24 : début / fin de trame
#define FRAME_UART_ESC     	((uint8_t)'\\') 					// 0x5C : caractère d’échappement
#define FRAME_UART_ESC_XOR	((uint8_t)0x20)

#define FRAME_SAS_GSE_SIZE		5   							// bytes
#define FRAME_GSE_MOTOR_SIZE	2   							// bytes
#define FRAME_MOTOR_GSE_SIZE    9								// bytes
#define FRAME_GSE_SAS_DATA_SIZE	15								// bytes
#define FRAME_GSE_SAS_SIZE      (FRAME_GSE_SAS_DATA_SIZE + 4) 	// bytes (included crc16 + start/end bit = 19 bytes)

/*=============================================================================
 COMPACT COMMAND BYTE
 Used for:
   SAS -> GSE (UART)
   GSE -> MOTOR (CAN)
=============================================================================*/
typedef struct {
	uint8_t estop	: 1;
	uint8_t vid		: 4;
	uint8_t hb	  	: 1;
	uint8_t seq	  	: 3; // 0..7
	uint8_t c1    	: 1; // Startup
	uint8_t c2    	: 1; // N2O Fill
	uint8_t c3    	: 1; // Igniter Start
	uint8_t c4    	: 1; // Motor Start
	uint8_t c5	  	: 1; // Optionnal command
	uint8_t c6	  	: 1; // Optionnal command
	uint8_t c7	  	: 1; // Optionnal command
	uint8_t crc8  	: 8; // CRC8
} cmd_bits_t;			 /* expected payload : 40 bits (start/end bit included) */

// Frame commands
typedef union {
	cmd_bits_t bits;
	uint8_t    byte;
} cmd_t;

/*=============================================================================
 MOTOR STATE FRAME
 (GSE <- MOTOR)
=============================================================================*/
typedef struct {
	uint8_t   	hb;                  	/* 1 bit */
	uint8_t		seq;                	/* 3 bits */
	uint8_t   	spark_plug_detector; 	/* E1 : 2 bit */
	uint8_t 	battery_percent;     	/* E2 : 7 bits */
	uint8_t 	command_states;      	/* E3 : 4 bits */
	uint16_t 	n2o_pressure;			/* E4 : 16 bits */
	uint16_t 	thrust_loadcell;		/* E5 : 12 bits */
	uint8_t 	n2o_dump_valve;      	/* E6 : 1 bits */
	uint8_t 	n2o_igniter_valve;   	/* E7 : 1 bits */
	uint8_t 	n2o_main_valve;      	/* E8 : 1 bits */
	uint8_t		logs;					/* E9 : 8 bits */
	uint16_t 	timestamp_ms;       	/* 16 bits */
} motor_state_frame_t;					/* expected payload : 72 bits */

/*=============================================================================
 GSE STATE FRAME
 (GSE -> SAS)
=============================================================================*/
typedef struct {
	uint8_t		vid;					/* 4 bits */
	uint8_t   	hb;                  	/* 1 bit */
	uint8_t 	seq;                	/* 3 bits */
	uint8_t		motor_data[8];			/* 64 bits */
	uint8_t 	command_states;      	/* E1 : 4 bits */
	uint16_t 	propellant_loadcell;	/* E2 : 12 bits */
	uint8_t		logs;					/* E3 : 8 bits */
	uint16_t 	timestamp_ms;       	/* 16 bits */
} gse_state_frame_t;					/* expected payload : 144 bits (start/end bit included) */

/*=============================================================================
 CAN IDENTIFIERS (11-bit)
=============================================================================*/
enum {
  CAN_ID_ESTOP        = 0x100,
  CAN_ID_STATUS_MIN   = 0x120,

  CAN_ID_CMD_MOTOR    = 0x200,   // DLC=1, data[0] = cmd_byte_t.byte
  CAN_ID_RSP_SHORT    = 0x210,
  CAN_ID_DATA_SEG     = 0x300,
};

/*=============================================================================
 FRAME API
=============================================================================*/

/*----------------------------
 Receive GSE <- SAS
----------------------------*/
bool Frame_UnpackGSEFromSASPayload(
		cmd_t *status,
		const uint8_t in[FRAME_SAS_GSE_SIZE]
);

/*----------------------------
 Transmit GSE -> MOTOR
 (Can be delete if we use directly SAS package)
----------------------------*/
cmd_t Frame_SetGSEToMotorPayload(
		bool estop,
		uint8_t seq,
		bool hb,
		bool c1,
		bool c2,
		bool c3,
		bool c4,
		bool c5,
		bool c6,
		bool c7
);

uint8_t Frame_PackGSEToMotorPayload(
		const cmd_t *status,
		uint8_t out[FRAME_GSE_MOTOR_SIZE]
);

/*----------------------------
 Receive GSE <- MOTOR
----------------------------*/
bool Frame_UnpackGSEFromMotorPayload(
		motor_state_frame_t *status,
		const uint8_t in[FRAME_MOTOR_GSE_SIZE]
);

/*----------------------------
 Transmit GSE -> SAS
----------------------------*/
gse_state_frame_t Frame_SetGSEToSASPayload(
		uint8_t seq,
		bool estop,
		uint8_t motor_data[FRAME_MOTOR_GSE_SIZE],
		uint8_t command_states,
		uint16_t propellant_loadcell,
		uint8_t logs,
		uint16_t timestamp_ms
);

uint8_t Frame_PackGSEToSASPayload(
		const gse_state_frame_t *status,
		uint8_t out[FRAME_GSE_SAS_SIZE]
);

