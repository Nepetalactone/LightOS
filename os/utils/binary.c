/*
 * binary.c
 *
 *  Created on: 14.03.2014
 *      Author: Christian
 */

#include "binary.h"
#include "../kernel/arch/address.h"


static inline uint64_t S_to_binary_(const char *s){
        uint32_t i = 0;
        while (*s) {
                i <<= 1;
                i += *s++ - '0';
        }
        return i;
}


//I hob die Datei wiederhergstellt weil sunsch die Methode im serial.c fählt.
uint8_t getBitValue(address a, offset o, uint32_t p){
	int mask = (1 << p);
	address location = address((base_address)a, o);
	int regValue = *location;
	int toCheck =  regValue & mask ;
	return toCheck && mask;
}
