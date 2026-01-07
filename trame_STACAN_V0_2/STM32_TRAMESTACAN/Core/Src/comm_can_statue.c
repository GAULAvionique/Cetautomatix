/*
 * comm_can_statue.c
 *
 *  Created on: 4 Jan 2026
 *      Author: Samuel
 */

#include "comm_can_statue.h"
#include <stdio.h>
#include <stdlib.h>

// data structure (private to this file)
struct Vsp{
	int8_t _val;
};
int check_pos(int* pos){
	if((pos < 0) || (pos > 1)){
		printf("Error: la position des valeurs ne peuvent être que 0 ou 1.\n");
		return 1;
	}else{return 0;};
};

Vsp* Vsp_create(int8_t* bloque_data){
	Vsp* p_vsp = (Vsp*)malloc(sizeof(struct Vsp));
	if (p_vsp != NULL){
		p_vsp->_val = bloque_data;
	}
	return p_vsp;
};
Vsp* Vsp_create(){
	int8_t bloque_data = 0;
return Vsp_create(&bloque_data);
};
Vsp* Vsp_create(int8_t valve1, int8_t valve2){
	int8_t bloque_data = (valve1 & 0x0F) | ((valve2 << 4) & 0xF0);
	return Vsp_create(&bloque_data);
};
void Vsp_destroy(Vsp* vsp){
	free(vsp);
};

void Vsp_set_val(Vsp* vsp, int8_t val, int n_valve){if (check_pos(&n_valve) == 1){
	if(n_valve == 0){
		/*set the second half of the _val with the correct val*/
		vsp->_val = (vsp->_val & 0xF0) | (val & 0x0F) ;
	}
	else{
		// val & 0x0F permet d'évité un débordement en ne concervant que les 4 dernier bits
		int8_t temp_val = (val & 0x0F) << 4;
		vsp->_val = (vsp->_val & 0x0F) | temp_val;
	};
}};

int8_t extractNibble(unsigned char reg, int pos){
	if (pos == 0){
		// lower nibble
		return reg & 0x0F;
	}else{
		// uper nibble
		return (reg >> 4) & 0x0F;
	};
};





