#include "command.h"


void BIT_SET(base_address a, offset o, uint32_t position){
	*((address)(a + o))  |= (1 << position);
}

void BIT_CLEAR(base_address a, offset o, uint32_t position){
	*((address)(a + o))  &= ~(1 << position);
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
