/*
 * bit_operation.c
 *
 *  Created on: 4 Jan 2026
 *      Author: Samuel
 */

#include <stdio.h>
int8_t extractNibble(unsigned char reg, int pos){
	if (pos == 0){
		// lower nibble
		return reg & 0x0F;
	}else{
		// uper nibble
		return (reg >> 4) & 0x0F;
	};
};



