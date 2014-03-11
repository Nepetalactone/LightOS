#include <stdio.h>
#include "executor.h"

void bit_operation(address a, offset o, command c, position p){
	switch(c){
		case SET:
		case ENABLE:
		case CLEAR:
			*(a + o)  |= (1 << p);
			break;
		case DISABLE:
			*(a + o)  &= ~(1 << p);
			break;
		case TOGGLE:
			*(a + o) ^= (1 << p);
			break;
		default:
			break;
	}
}

void byte_operation(address a, offset o, command c, position p, byte b){
	switch(c){
		case SET:
			//TODO
			break;
		case CLEAR:
			//TODO
			break;
		default:
			break;
	}
}

unsigned int getBitValue(address a, offset o, position p){
	int bitValue = *(a + o) & (1 << p);
	return bitValue;
}

