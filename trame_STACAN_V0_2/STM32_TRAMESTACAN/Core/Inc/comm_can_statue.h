/*
 * comm_can_statue.h
 *
 *  Created on: 4 Jan 2026
 *      Author: Samuel
 */
#include <stdio.h>

#ifndef INC_COMM_CAN_STATUE_H_
#define INC_COMM_CAN_STATUE_H_

int8_t extractNibble(unsigned char reg, int pos);

typedef struct Vsp Vsp;//ValveStatePaire ValveStatePaire;
typedef struct Seg1331 Seg1331;   // segment de 2 élément d'un bit et 2 élément de 3 bit.

Seg1331* seg1331_create();
void seg1331_destroy(Seg1331* seg);

/* pos == 0 ou pos == 1 */
void seg1331_set1(Seg1331*, int8_t val, int pos);
void seg1331_set3(Seg1331*, int8_t val, int pos);// set the value (of 3 bit len).

// constructeur
Vsp* Vsp_create();
Vsp* Vsp_create(int8_t* bloque_data);
Vsp* Vsp_create(int8_t valve1, int8_t valve2);

// destructeur
void Vsp_destroy(Vsp* vsp);

void Vsp_set_val(Vsp* vsp, int8_t val, int n_valve);
int8_t Vsp_get_val(Vsp* vsp, int n_valve);

typedef struct Pcs Pcs;   // Pcs: Paquet_can_statue

// functions for Statue_can
Pcs* pcs_create(unsigned char id_can);    // constructor
void pcs_destroy(Pcs* p);	// destructeur
int pcs_get_nb_bytes(Pcs* p);

// get the diverse value
int8_t pcs_get_IPA_VM(Pcs* p);
int8_t pcs_get_IPA_VI(Pcs* p);
int8_t pcs_get_SEQ(Pcs* p);
int8_t pcs_get_HB(Pcs* p);
int8_t pcs_get_CI(Pcs* p);
int8_t pcs_get_AB(Pcs* p);// vrai longueur: 3 bits
int8_t pcs_get_EDC(Pcs* p);// vrai longueur: 4 bits
int8_t pcs_get_N2O_VSD(Pcs* p); // vrai longueur: 4 bits
int8_t pcs_get_N2O_VI(Pcs* p); // vrai longueur: 4 bits
int8_t pcs_get_N2O_VM(Pcs* p); // vrai longueur: 4 bits
int16_t pcs_get_N2O_SP(Pcs* p); // vrai longueur: 16 bits
int16_t pcs_get_T(Pcs* p); // vrai longueur: 16 bits


/* set the value of the diverse value
 * (by 'set', it's mean to make a copy of the value*/
void pcs_set_IPA_VM(Pcs* p, int8_t ipa_mv); // 4 bit
void pcs_set_IPA_VI(Pcs* p, int8_t ipa_vi); // 4 bit
void pcs_set_SEQ(Pcs* p, int8_t seq);// 3 bit
void pcs_set_HB(Pcs* p, int8_t hb);// 1 bit
void pcs_set_CI(Pcs* p, int8_t ci);// // 1 bit
void pcs_set_AB(Pcs* p, int8_t ab);// vrai longueur: 3 bits
void pcs_set_EDC(Pcs* p, int8_t edc);// vrai longueur: 4 bits
void pcs_set_N2O_VSD(Pcs* p, int8_t N2O_vsd); // vrai longueur: 4 bits
void pcs_set_N2O_VI(Pcs* p, int8_t N2O_vi); // vrai longueur: 4 bits
void pcs_set_N2O_VM(Pcs* p, int8_t N2O_vm); // vrai longueur: 4 bits

void pcs_update_N2O_SP(Pcs* p, int16_t* N2O_SP); // vrai longueur: 16 bits
void pcs_update_T(Pcs* p, int16_t* TimeStamp); // vrai longueur: 16 bits
void pcs_update_N2O_VM(Pcs* p, int8_t* N2O_VM); // vrai longueur: 4 bits

void pcs_set_N2O_SP(Pcs* p, int16_t N2O_SP); // vrai longueur: 16 bits
void pcs_set_T(Pcs* p, int16_t TimeStamp); // vrai longueur: 16 bits

uint8_t pcs_send_can_data(Pcs* p, data_container);    // TODO: set the type for the datacontainer



#endif /* INC_COMM_CAN_STATUE_H_ */
