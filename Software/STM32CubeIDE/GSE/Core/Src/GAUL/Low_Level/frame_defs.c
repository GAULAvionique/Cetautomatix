#include <GAUL/Low_Level/frame_defs.h>
#include <string.h>


/*=============================================================================
 LOW LEVEL
=============================================================================*/

/*----------------------------
 CRC16
----------------------------*/
static uint16_t COMM_GenerateCRC16(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}


/*=============================================================================
 HIGH LEVEL
=============================================================================*/

/*----------------------------
 Receive GSE <- SAS
----------------------------*/
/*
 * [0] $
 * [1] estop + vid + seq
 * [2] hb + c1 + ... + c7
 * [3] crc8
 * [4] $
*/
bool Frame_UnpackGSEFromSASPayload(cmd_t *status, const uint8_t in[FRAME_SAS_GSE_SIZE]) {
	if(!status || !in) return false;
	if(in[0] != FRAME_UART_FLAG || in[FRAME_SAS_GSE_SIZE] != FRAME_UART_FLAG) return false;

	status->bits.estop	= in[1] & 0xE0u;
	status->bits.vid	= in[1] & 0x78u;
	status->bits.seq 	= in[1] & 0x07u;
	status->bits.hb		= in[2] & 0x80u;
	status->bits.c1 	= in[2] & 0x40u;
	status->bits.c2 	= in[2] & 0x20u;
	status->bits.c3 	= in[2] & 0x10u;
	status->bits.c4		= in[2] & 0x08u;
	status->bits.c5 	= in[2] & 0x04u;
	status->bits.c6 	= in[2] & 0x02u;
	status->bits.c7		= in[2] & 0x00u;
	status->bits.crc8	= in[3];

	return true;
}


/*----------------------------
 Transmit GSE -> MOTOR
 (Can be delete if we use directly SAS package)
----------------------------*/
cmd_t Frame_SetGSEToMotorPayload(bool estop, uint8_t seq, bool hb, bool c1, bool c2, bool c3, bool c4, bool c5, bool c6, bool c7) {
	cmd_t status;

	status.bits.estop	= (uint8_t)(estop ? 0x01u : 0x00u);
	status.bits.vid		= (uint8_t)PROTO_VER;
	status.bits.seq		= (uint8_t)(seq & 0x07u);
	status.bits.hb		= (uint8_t)(hb ? 0x01u : 0x00u);
	status.bits.c1		= (uint8_t)(c1 ? 0x01u : 0x00u);
	status.bits.c2		= (uint8_t)(c2 ? 0x01u : 0x00u);
	status.bits.c3		= (uint8_t)(c3 ? 0x01u : 0x00u);
	status.bits.c4		= (uint8_t)(c4 ? 0x01u : 0x00u);
	status.bits.c5		= (uint8_t)(c5 ? 0x01u : 0x00u);
	status.bits.c6		= (uint8_t)(c6 ? 0x01u : 0x00u);
	status.bits.c7		= (uint8_t)(c7 ? 0x01u : 0x00u);

	return status;
}

/*
 * [0] estop + vid + seq
 * [1] hb + c1 + ... + c7
*/
uint8_t Frame_PackGSEToMotorPayload(const cmd_t *status, uint8_t out[FRAME_GSE_MOTOR_SIZE]) {
	if(!status || !out) return -1;

	out[0] = (uint8_t)((status->bits.estop << 7) | (status->bits.vid << 4) | status->bits.seq);
	out[1] = (uint8_t)((status->bits.hb << 7) | (status->bits.c1 << 6) | (status->bits.c2 << 5) | (status->bits.c3 << 4) | (status->bits.c4 << 3) | (status->bits.c5 << 2) | (status->bits.c6 << 1) | status->bits.c7);

	return 0;
}

/*----------------------------
 Receive GSE <- MOTOR
----------------------------*/
/*
 * [0] hb + battery_percent
 * [1] seq + spark_plug_detector + n2o_dump_valve + n2o_igniter_valve + n2o_main_valve
 * [2] n2o_pressure (MSB)
 * [3] n2o_pressure (LSB)
 * [4] thrust_loadcell (MSB - 8)
 * [5] thrust_loadcell (LSB - 4) + command_states
 * [6] logs
 * [7] timestamp_ms (MSB)
 * [8] timestamp_ms (LSB)
*/
bool Frame_UnpackGSEFromMotorPayload(motor_state_frame_t *status, const uint8_t in[FRAME_MOTOR_GSE_SIZE]) {
	if(!status || !in) return false;

	status->hb 					= in[0] & 0x80u;
	status->battery_percent 	= in[0] & 0x7Fu;
	status->seq 				= in[1] & 0xE0u;
	status->spark_plug_detector = in[1] & 0x18u;
	status->n2o_dump_valve 		= in[1] & 0x04u;
	status->n2o_igniter_valve 	= in[1] & 0x02u;
	status->n2o_main_valve 		= in[1] & 0x01u;
	status->n2o_pressure 		= (uint16_t)(in[2] << 8) | in[3];
	status->thrust_loadcell 	= (uint16_t)(in[4] << 8) | (in[5] & 0xF0u);
	status->command_states 		= in[5] & 0x0Fu;
	status->logs				= in[6];
	status->timestamp_ms 		= (uint16_t)(in[7] << 8) | in[8];

	return true;
}

/*----------------------------
 Transmit GSE -> SAS
----------------------------*/
gse_state_frame_t Frame_SetGSEToSASPayload(uint8_t seq, bool hb, uint8_t motor_data[FRAME_MOTOR_GSE_SIZE], uint8_t command_states, uint16_t propellant_loadcell, uint8_t logs, uint16_t timestamp_ms) {
	gse_state_frame_t status;

	status.vid					= (uint8_t)PROTO_VER;
	status.seq					= (uint8_t)(seq & 0x07u);
	status.hb               	= (uint8_t)(hb ? 0x01u : 0x00u);
	memcpy(status.motor_data, motor_data, FRAME_MOTOR_GSE_SIZE);
	status.command_states   	= (uint8_t)(command_states & 0x0Fu);
	status.propellant_loadcell 	= (uint16_t)(propellant_loadcell & 0xFFFu);
	status.logs					= logs;
	status.timestamp_ms     	= timestamp_ms;

	return status;
}

/*
 * [0] 	  $
 * [1] 	  vid + seq + hb
 * [2-10] motor_data (MSE to LSB)
 * [11]   propellant_loadcell (MSB - 8)
 * [12]   propellant_loadcell (LSB - 4) + command_states
 * [13]   logs
 * [14]   timestamp_ms (MSB)
 * [15]   timestamp_ms (LSB)
 * [16]   crc16 (MSB)
 * [17]   crc16 (LSB)
 * [18]   $
*/
uint8_t Frame_PackGSEToSASPayload(const gse_state_frame_t *status, uint8_t out[FRAME_GSE_SAS_SIZE]) {
	if(!status || !out) return -1;

	// Payload
	uint8_t data[FRAME_GSE_SAS_DATA_SIZE] = { 0 };
	data[0] = (uint8_t)((status->vid << 4) | (status->seq << 1) | status->hb);
	for(int i = 1; i <= FRAME_MOTOR_GSE_SIZE; i++) {
		data[i] = status->motor_data[i];
	}
	data[10] = (uint8_t)(status->propellant_loadcell & 0x0FF0u);
	data[11] = (uint8_t)((status->propellant_loadcell & 0x000Fu) << 4) | status->command_states;
	data[12] = (uint8_t)(status->logs);
	data[13] = (uint8_t)(status->timestamp_ms & 0xFF00u);
	data[14] = (uint8_t)(status->timestamp_ms & 0x00FFu);
	// Generate CRC16 on data only
	uint16_t crc16 = COMM_GenerateCRC16((const uint8_t *)data, FRAME_GSE_SAS_DATA_SIZE);

	// Pack full payload
	out[0] = FRAME_UART_FLAG;
	for(int d = 0; d < FRAME_GSE_SAS_DATA_SIZE; d++) {
		out[d + 1] = data[d];
	}
	out[FRAME_GSE_SAS_DATA_SIZE - 3] = (uint8_t)(crc16 & 0xFF00u);
	out[FRAME_GSE_SAS_DATA_SIZE - 2] = (uint8_t)(crc16 & 0x00FFu);
	out[FRAME_GSE_SAS_DATA_SIZE - 1] = FRAME_UART_FLAG;

	return 0;
}









// TODO: move to motor project
/*
motor_state_frame_t Frame_SetMotorToGSEPayload(uint8_t seq, bool hb, uint8_t spark_plug_detector, uint8_t battery_percent, uint8_t command_states, uint16_t n2o_pressure, uint16_t thrust_loadcell, bool n2o_dump_valve, bool n2o_igniter_valve, bool n2o_main_valve, uint16_t timestamp_ms) {
	motor_state_frame_t status;

	status.seq                 	= (uint8_t)(seq & 0x07u);
	status.hb                  	= (uint8_t)(hb ? 0x01u : 0x00u);
	status.spark_plug_detector 	= (uint8_t)(spark_plug_detector & 0x3u);
	status.battery_percent     	= (uint8_t)(battery_percent & 0x7Fu);
	status.command_states      	= (uint8_t)(command_states & 0x0Fu);
	status.n2o_pressure        	= n2o_pressure;
	status.thrust_loadcell		= (uint16_t)(thrust_loadcell & 0xFFFu);
	status.n2o_dump_valve      	= (uint8_t)(n2o_dump_valve ? 0x01u : 0x00u);
	status.n2o_igniter_valve   	= (uint8_t)(n2o_igniter_valve ? 0x01u : 0x00u);
	status.n2o_main_valve      	= (uint8_t)(n2o_main_valve ? 0x01u : 0x00u);
	status.timestamp_ms        	= timestamp_ms;

	return status;
}
*/

/*
 * [0] hb + battery_percent
 * [1] seq + spark_plug_detector + n2o_dump_valve + n2o_igniter_valve + n2o_main_valve
 * [2] n2o_pressure (MSB)
 * [3] n2o_pressure (LSB)
 * [4] thrust_loadcell (MSB - 8)
 * [5] thrust_loadcell (LSB - 4) + command_states
 * [6] timestamp_ms (MSB)
 * [7] timestamp_ms (LSB)
*/
/*
uint8_t Frame_PackMotorToGSEPayload(const motor_state_frame_t *status, uint8_t out[MOTOR_FRAME_SIZE]) {
	if(!status || !out) return -1;

	out[1] = (uint8_t)((status->seq << 6) | (status->spark_plug_detector << 3) | (status->n2o_dump_valve << 2) | (status->n2o_igniter_valve << 1) | status->n2o_main_valve);
	out[2] = (uint8_t)(status->n2o_pressure & 0xFF00u);
	out[3] = (uint8_t)(status->n2o_pressure & 0x00FFu);
	out[4] = (uint8_t)(status->thrust_loadcell & 0x0FF0u);
	out[5] = (uint8_t)(((status->thrust_loadcell & 0x000Fu) << 4) | status->command_states);
	out[6] = (uint8_t)(status->timestamp_ms & 0xFF00u);
	out[7] = (uint8_t)(status->timestamp_ms & 0x00FFu);

	return 0;
}
*/
