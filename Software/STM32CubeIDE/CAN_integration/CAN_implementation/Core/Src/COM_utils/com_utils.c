/*
 * com_utils.c
 *
 *  Created on: 4 Mar 2026
 *      Author: Samuel
 */
#include "com_utils.h"
#include "../../TAGCAN/include/sub_include/data_container.h"
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

CAN_RxHeaderTypeDef RxHeader;
uint8_t             RxData[8];

// valeurs
int8_t val_SEQ = 0;
int8_t val_HB = 0;
int8_t val_CI = 0;
int8_t val_AB = 0;
int8_t val_EDC = 0;
int16_t val_N2OPS = 0;
int16_t val_TLC = 0;
int8_t val_N2ODVS = 0;
int8_t val_N2OIV = 0;
int8_t val_N2OMV = 0;
int16_t val_TIME = 0;
int8_t out_Rx[8] = {0,0,0,0,0,0,0,0};


int datacheck = 0;    // permet d'update les valeurs uniquement lors de la reception de valeurs
// module pour CAN

#ifndef INC_COM_UTILS_H_
int8_t TxData_moteur[8] = {0,0,0,0,0,0,0,0};
int8_t RxData_moteur[8] = {0,0,0,0,0,0,0,0};
int32_t TxMailbox;
#endif
///* initialise tout les fonction de communication. (point d'appel unique: main) */
void init_com(void){
	// init module extene
	CAN_init_TAGCAN();
	CAN_setup_TAG();
	// init CAN (interne)
	CAN_setup_DF();

	// init msg
	if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING)!=HAL_OK)
	{
		Error_Handler();
	}
}


void CAN_init_TAGCAN(void){
	init_data_container();
	init_tag_manager();
}

void CAN_setup_DF(int32_t std_id){
	std_id &= 0x7FFU;
	TxHeader.IDE = CAN_ID_STD;		// Standard ID (11 bits)
	TxHeader.StdId = std_id;			// id
	TxHeader.RTR = CAN_RTR_DATA;	// sending data
	TxHeader.DLC = 8;				// nb of bytes
}



void CAN_setup_TAG(void){
	set_tag("SEQ", 3);		//*>Compteur de séquences 	*/
	set_tag("HB",1);		//*>Watchdog HeartBeat 		*/
	set_tag("CI",2);		//*>Continuité igniter 		*/
	set_tag("AB", 7);		//*>Alim. Batetterie (%) 	*/
	set_tag("EDC", 4);		//*>États des commandes 	*/
	set_tag("N2OPS",16);	//*>N2O Pressure Sensor 	*/
	set_tag("TLC",12);		//*>Thrust Load Cell 		*/
	set_tag("N2ODVS",1);	//*>N2O Dump Valve State 	*/
	set_tag("N2OIV",1);		//*>N2O Igniter Valve 		*/
	set_tag("N2OMV",1);		//*>N2O Main Valve 			*/
	set_tag("TIME",16);		//*>Timestamp (ms) 			*/
}


CAN_FilterTypeDef* CAN_setup_Filter(int32_t minimum_id, int32_t maximum_id, int filterBank){
	// understanding this https://youtu.be/JfWlIY0zAIc
	CAN_FilterTypeDef canfilterconfig;
	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank = filterBank;		// Assign and Select Filter Banks
	canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canfilterconfig.FilterIdHigh = maximum_id <<5;
	canfilterconfig.FilterMaskIdHigh = maximum_id << 5;
	canfilterconfig.FilterIdLow = minimum_id;
	canfilterconfig.FilterMaskIdLow = minimum_id;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;    // 16 ou 32 bits register
	canfilterconfig.SlaveStartFilterBank = 20;    // how many filters to assign to the CAN1 (master can)

	HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);
	return &canfilterconfig;
}



///////////////////////////////////////////////////////////////
//--------------!!!!-DANS LE MAIN LOOP!!!!!--------------------
///////////////////////////////////////////////////////////////





void main_loop(void){
	update_all_val_send();
	CAN_send_msg();
}

void update_TagData(void){
	if(datacheck == 1){
		change_val_to_TagData(&out_Rx);
	}
}

void CAN_send_msg(void){
	if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox)!=HAL_OK){
		Error_Handler();
	}
}

void CAN_change_val(void){

}

void update_val(const char* nom_du_tag, int32_t val){
	set_tag(nom_du_tag, val);
}

void update_all_val_send(void){	//TODO: changer toute les constantes ici par la source actuelle de donné (ou ne jamais appeller cette fonction si c'est le recever)
	set_value("SEQ", 3);		//*>Compteur de séquences 	*/
	set_value("HB", 1);		//*>Watchdog HeartBeat 		*/
	set_value("CI", 2);		//*>Continuité igniter 		*/
	set_value("AB", 7);		//*>Alim. Batetterie (%) 	*/
	set_value("EDC", 4);		//*>États des commandes 	*/
	set_value("N2OPS", 16);	//*>N2O Pressure Sensor 	*/
	set_value("TLC", 12);		//*>Thrust Load Cell 		*/
	set_value("N2ODVS", 1);	//*>N2O Dump Valve State 	*/
	set_value("N2OIV", 1);		//*>N2O Igniter Valve 		*/
	set_value("N2OMV", 1);		//*>N2O Main Valve 			*/
	set_value("TIME", 16);		//*>Timestamp (ms) 			*/
}


void update_all_val_receve(void){

}








////////////////////////////////////////////////////////
// événementielle
///////////////////////////////////////////////////////



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    Error_Handler();
  }
  if ((RxHeader.StdId == 0x103))    // suposé être l'id de l'autre CAN
  {
	  datacheck = 1;
	  for (int i = 0; i < N_BYTES; i++){
		  out_Rx[i] = RxData[i];
	  }

	  // update du conteneur de donnée.
	  //change_val_to_TagData(&RxData);
  }
}








