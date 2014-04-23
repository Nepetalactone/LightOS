#include "command.h"


void BIT_SET(base_address a, offset o, uint32_t position){
	*((address)(a + o))  |= (1 << position);
}

void BIT_CLEAR(base_address a, offset o, uint32_t position){
	*((address)(a + o))  &= ~(1 << position);
}

uint32_t BIT_READ(base_address a, offset o, uint8_t start, uint8_t range){

	//TODO: BIT_READ bit_mask (command.h) as inputparam
	//uint32_t bitmask = BIT_MASK(01111111);
	//uint32_t full = *(a+o);
	//uint32_t withmask = qwer & bitmask;

	return (*((address)(a + o)) >> 0) & ((1 << range)-1);
}

void BIT_TOGGLE(base_address a, offset o, uint32_t position){
	*((address)(a + o)) ^= (1 << position);
}

void REG_SET(base_address a, offset o, uint32_t position){
	*((address)(a + o)) = position;
}

void REG_CLEAR(base_address a, offset o){
	*((address)(a + o)) = 0x0;
}

char UART_READ(base_address a, offset o, char character){
	return *((address)(a + o));
}

void UART_WRITE(base_address a, offset o, char character){
	*((address)(a + o)) = character;
}

void WAIT_FOR_WRITE(base_address a, offset o, uint32_t position){
	//TODO wait for bit/register set
}
