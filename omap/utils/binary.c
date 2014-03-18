/*
 * binary.c
 *
 *  Created on: 14.03.2014
 *      Author: Christian
 */

#include "binary.h"
#include "../kernel/arch/address.h"


static inline uint64_t S_to_binary_(const char *s)
{
        uint64_t i = 0;
        while (*s) {
                i <<= 1;
                i += *s++ - '0';
        }
        return i;
}

uint32_t getBitValue(address a, offset o, position p){
	int mask = (1 << p);
	address location = __CONCRETE_ADDRESS((base_address)a, o);
	int regValue = *location;
	int toCheck =  regValue & mask ;
	return toCheck && mask;
}


