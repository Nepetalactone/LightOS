#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdint.h>
#include "address.h"

typedef volatile unsigned char command;


/*
	executes a command on the bit/byte in register (at given position) pointed to by given address + offset
	a = base address
	o = address offset
	value = position of the bit, or value to write into register
	returns values if command was a read command
*/
uint32_t exec_command(base_address a, offset o, command c, uint32_t value);

void BIT_SET(base_address a, offset o, uint32_t value);
void BIT_CLEAR(base_address a, offset o, uint32_t value);
uint32_t BIT_READ(base_address a, offset o, uint8_t start, uint8_t range);
void BIT_TOGGLE(base_address a, offset o, uint32_t value);
void REG_SET(base_address a, offset o, uint32_t value);
void REG_CLEAR(base_address a, offset o);
char UART_READ(base_address a, offset o, char character);
void UART_WRITE(base_address a, offset o, char character);
/*
	a = base address
	o = address offset
	p = position of the bit
	returns the value of the bit at given position (p) in the register pointed to by given address
*/
uint8_t getBitValue(address a, offset o, uint32_t p);


#endif
