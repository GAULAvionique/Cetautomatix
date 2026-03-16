#include "GAUL_Drivers/comm.h"


/*=============================================================================
 LOW LEVEL
=============================================================================*/

/*----------------------------
 CRC8
----------------------------*/
static bool COMM_CheckCRC8(const uint8_t *data, size_t len, uint8_t expected_crc) {
    uint8_t crc = 0x00;

    for(uint16_t i = 0; i < len; i++) {
        crc ^= data[i];
        for(uint8_t j = 0; j < 8; j++) {
            if(crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }

    return (crc == expected_crc);
}

/*----------------------------
 Sequence (SEQ)
----------------------------*/
static inline uint8_t COMM_NextSEQ(comm_gse_t *dev) {
	uint8_t ret = dev->seq;
	dev->seq = (dev->seq + 1) & 0x07;

	return ret;
}

static uint8_t COMM_CheckSEQ(uint8_t prev_seq, uint8_t now_seq) {
	return ((prev_seq + 1) & 0x07) == now_seq;
}

// TODO: add safe vid check
/*----------------------------
 Check Frame Version (VID)
----------------------------*/
static bool COMM_CheckVID(comm_gse_t *dev, const uint8_t vid) {
	bool check;
	if(vid == (uint8_t)PROTO_VER) {
		dev->isVID = true;
		check = true;
	} else {
		dev->isVID = false;
		check = false;
	}

	return check;
}


/*=============================================================================
 HIGH LEVEL
=============================================================================*/

/*----------------------------
 Init COMM GSE
----------------------------*/
int8_t COMM_Init(comm_gse_t *dev) {
	if(!dev || !dev->can_dev || !dev->rfd_dev) return -1;

	dev->isVID 			= false;
	dev->seq 			= 0;
	dev->last_sas_seq 	= 0;
	dev->last_motor_seq = 0;
	// maybe init rfd900x/can here or in main

	return 0;
}

/*----------------------------
 Receive GSE <- SAS
----------------------------*/
int8_t COMM_ReceiveGSEFromSAS(comm_gse_t *dev, cmd_t *cmd) {
	if(!dev) return -1;

	// check if data ready in ring_buffer
	if(!dev->rfd_dev->uart->data_ready) return 0;
	// check if complete frame is in ring_buffer
	if(RFD900x_Available(dev->rfd_dev) < FRAME_SAS_GSE_SIZE) return 0;
	dev->rfd_dev->uart->data_ready = false;

	uint8_t commands[FRAME_SAS_GSE_SIZE];
	if(RFD900x_ReadFrame(dev->rfd_dev, commands, FRAME_SAS_GSE_SIZE) == false) return -1;

	if(Frame_UnpackGSEFromSASPayload(cmd, commands) != true) return -1;

	if(COMM_CheckCRC8(commands, FRAME_SAS_GSE_SIZE, cmd->bits.crc8) != true) return -1;

	if(!dev->isVID) {
		bool check = COMM_CheckVID(dev, cmd->bits.vid);
		if(!check) {
			return -1;
		}
	}

	if(!COMM_CheckSEQ(dev->last_sas_seq, cmd->bits.seq)) {
		// TODO: add log when skip sequence (not return)
		return -1;
	}
	dev->last_sas_seq = cmd->bits.seq;
	dev->seq = dev->last_sas_seq;

	if(cmd->bits.estop) COMM_TransmitGSEToMotor(dev, cmd);

	return 0;
}

/*----------------------------
 Transmit GSE -> MOTOR
----------------------------*/
int8_t COMM_TransmitGSEToMotor(comm_gse_t *dev, const cmd_t *cmd) {
	if(!dev) return -1;

    if(cmd->bits.estop) {
    	CAN_FIFO_SetTXHeader(dev->can_dev, CAN_ID_ESTOP, FRAME_GSE_MOTOR_SIZE);
    } else {
    	CAN_FIFO_SetTXHeader(dev->can_dev, CAN_ID_CMD_MOTOR, FRAME_GSE_MOTOR_SIZE);
    }

    /*
    uint8_t next_motor_seq = COMM_NextSEQ(dev);
    cmd->bits.seq = next_motor_seq;
    */

    uint8_t commands[FRAME_GSE_MOTOR_SIZE] = {0};
    if(Frame_PackGSEToMotorPayload(cmd, commands) != 0) return -1;

    if(CAN_FIFO_Send(dev->can_dev, commands) != HAL_OK) return -1;

    return 0;
}

// TODO: move to motor project
/*----------------------------
 Transmit GSE <- MOTOR
----------------------------*/
/*
int8_t COMM_ReceiveGSEFromMotor(comm_gse_t *dev, const motor_state_frame_t *status) {
	if(!dev) return -1;

	COMM_SetTXHeader(dev, CAN_ID_STATUS_MIN, FRAME_MOTOR_GSE_SIZE);

	if(dev->hcan->State != HAL_CAN_STATE_READY) {
		return -2;
	}

	uint8_t commands[FRAME_MOTOR_GSE_SIZE] = {0};
	if(Frame_PackMotorToGSEPayload(status, commands) != 0) return -1;

	return (HAL_CAN_AddTxMessage(dev->hcan, dev->header_tx, commands, dev->mailbox_tx) == HAL_OK) ? 0 : -1;
}
*/

/*----------------------------
 Receive GSE <- MOTOR
----------------------------*/
int8_t COMM_ReceiveGSEFromMotor(comm_gse_t *dev, motor_state_frame_t *status) {
    if(!dev) return -1;

    static uint8_t rx_buffer[FRAME_MOTOR_GSE_SIZE];
    can_frame_t frame;

    while(CAN_FIFO_Available(dev->can_dev) > 0) {
        if(!CAN_FIFO_ReadFrame(dev->can_dev, &frame)) break;

        if(frame.id == CAN_ID_DATA_SEG) {
            memcpy(&rx_buffer[0], frame.data, 8);
        } else if(frame.id == CAN_ID_DATA_SEG + 1) {
            rx_buffer[8] = frame.data[0];

            if(Frame_UnpackGSEFromMotorPayload(status, rx_buffer) == true) return 0;
        }
    }
    return -1;
}

/*----------------------------
 Transmit GSE -> SAS
----------------------------*/
int8_t COMM_TransmitGSEToSAS(comm_gse_t *dev, const gse_state_frame_t *status) {
	if(!dev) return -1;

	static uint8_t frame[FRAME_GSE_SAS_SIZE] = {0};
    if(Frame_PackGSEToSASPayload(status, frame) != 0) return -1;

    if(!COMM_CheckSEQ(dev->last_motor_seq, status->seq)) {
		// TODO: add log when skip sequence (not return)
		return -1;
	}
	dev->last_motor_seq = status->seq;
	dev->seq = dev->last_motor_seq;

    if(RFD900x_Send(dev->rfd_dev, frame, FRAME_GSE_SAS_SIZE) != 0) return -1;

    return 0;
}


/*=============================================================================
  UTILITIES
=============================================================================*/

/*----------------------------
 Getter SEQ
----------------------------*/
uint8_t COMM_GetSEQ(comm_gse_t *dev) {
	return dev->seq;
}

/* ------------------------------------------------------------- */
/* WAIT CAN RX */
/* ------------------------------------------------------------- */
/*
bool wait_can_rx(CAN_HandleTypeDef *hcan,
                        CAN_RxHeaderTypeDef *rxh,
                        uint8_t *data,
                        uint32_t timeout_ms)
{
    uint32_t t0 = HAL_GetTick();

    while (HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) == 0)
    {
        if ((HAL_GetTick() - t0) > timeout_ms)
            return false;
    }

    return HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, rxh, data) == HAL_OK;
}
*/

/* ------------------------------------------------------------- */
/* UART UNSTUFF */
/* ------------------------------------------------------------- */
/*
bool uart_unstuff_between_flags(const uint8_t *frame,
                                       uint16_t frame_len,
                                       uint8_t *decoded,
                                       uint16_t *decoded_len)
{
    if (!frame || !decoded || !decoded_len)
        return false;

    if (frame_len < 2)
        return false;

    if (frame[0] != TRAMES_FLAG || frame[frame_len-1] != TRAMES_FLAG)
        return false;

    uint16_t out_i = 0;

    for (uint16_t i = 1; i < frame_len-1; i++)
    {
        if (frame[i] == SAS_UART_ESC)
        {
            if ((i+1) >= frame_len-1)
                return false;

            decoded[out_i++] = frame[i+1] ^ SAS_UART_ESC_XOR;
            i++;
        }
        else
        {
            decoded[out_i++] = frame[i];
        }
    }

    *decoded_len = out_i;
    return true;
}
*/

/* ------------------------------------------------------------- */
/* PARSE UART COMMAND */
/* ------------------------------------------------------------- */

/*
bool parse_uart_cmd_frame(const uint8_t *frame,
                                 uint16_t frame_len,
                                 uint8_t *cmd_byte_out)
{
    uint8_t decoded[4];
    uint16_t decoded_len = 0;

    if (!uart_unstuff_between_flags(frame, frame_len, decoded, &decoded_len))
        return false;

    if (decoded_len != 2)
        return false;

    uint8_t cmd = decoded[0];
    uint8_t crc = decoded[1];

    if (comm_crc8_atm(&cmd,1) != crc)
        return false;

    *cmd_byte_out = cmd;

    return true;
}
*/

/* ------------------------------------------------------------- */
/* PARSE UART STATUS */
/* ------------------------------------------------------------- */

/*
bool parse_uart_motor_status_frame(const uint8_t *frame,
                                          uint16_t frame_len,
                                          motor_status_t *st_out)
{
    uint8_t decoded[12];
    uint16_t decoded_len = 0;

    uint8_t raw[10];

    if (!uart_unstuff_between_flags(frame,frame_len,decoded,&decoded_len))
        return false;

    if (decoded_len != 9)
        return false;

    if (comm_crc8_atm(decoded,8) != decoded[8])
        return false;

    raw[0] = TRAMES_FLAG;

    memcpy(&raw[1],decoded,8);

    raw[9] = TRAMES_FLAG;

    return motor_status_unpack(st_out,raw);
}
*/

/* ------------------------------------------------------------- */
/* EXTRACT COMMAND STATES */
/* ------------------------------------------------------------- */

/*
uint8_t cmd_states_from_cmd_byte(uint8_t cmd_byte)
{
    return (cmd_byte >> 4) & 0x0F;
}
*/

/* ------------------------------------------------------------- */
/* RECEIVE MOTOR STATUS FROM CAN */
/* ------------------------------------------------------------- */

/*
int recv_motor_status_from_can(CAN_HandleTypeDef *hcan,
                                      motor_status_t *st,
                                      uint32_t timeout_ms)
{
    CAN_RxHeaderTypeDef rxh;
    uint8_t data[8];

    uint8_t raw[10];

    bool got_seg0 = false;
    bool got_seg1 = false;

    uint32_t t0 = HAL_GetTick();

    while ((!got_seg0 || !got_seg1) &&
           ((HAL_GetTick() - t0) <= timeout_ms))
    {
        if (!wait_can_rx(hcan,&rxh,data,10))
            continue;

        if (rxh.StdId == (CAN_ID_STATUS_MIN & 0x7FF) && rxh.DLC == 8)
        {
            memcpy(&raw[0],data,8);
            got_seg0 = true;
        }
        else if (rxh.StdId == ((CAN_ID_STATUS_MIN+1) & 0x7FF) && rxh.DLC == 2)
        {
            raw[8] = data[0];
            raw[9] = data[1];
            got_seg1 = true;
        }
    }

    if (!got_seg0 || !got_seg1)
        return -1;
    return motor_status_unpack(st,raw) ? 0 : -1;
}
*/

