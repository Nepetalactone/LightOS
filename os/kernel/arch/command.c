#include "command.h"
#include "uart.h"


void BIT_SET(base_address b, offset o, uint32_t position){
	*((address)(b + o))  |= (1 << position);
}

void BIT_CLEAR(base_address b, offset o, uint32_t position){
	*((address)(b + o))  &= ~(1 << position);
}

uint32_t BIT_READ(base_address b, offset o, uint8_t start, uint8_t range){

	//TODO: BIT_READ bit_mask (command.h) as inputparam
	//uint32_t bitmask = BIT_MASK(01111111);
	//uint32_t full = *(IRQ_LINE);
	//uint32_t withmask = qwer & bitmask;

	return (*((address)(b + o)) >> 0) & ((1 << range)-1);
}

void BIT_TOGGLE(base_address b, offset o, uint32_t position){
	*((address)(b + o)) ^= (1 << position);
}

void REG_SET(base_address b, offset o, uint32_t value){
	*((address)(b + o)) = value;
}

void REG_SET_RANGE(base_address b, offset o, uint32_t value, uint8_t startPos, uint8_t size) {
	*((address)(b+o+startPos)) = value;
}

void REG_CLEAR(base_address b, offset o){
	*((address)(b + o)) = 0x0;
}

char UART_READ(base_address b, offset o, char character){
	return *((address)(b + o));
}

void UART_WRITE(base_address b, offset o, char character){
	*((address)(b + o)) = character;
}

void WAIT_FOR_WRITE(base_address b, offset o, uint32_t position){
	while(getBitValue((address)UART03, SYSS, position) == 0x0) {
			// wait till set is done
	}
}
