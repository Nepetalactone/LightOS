#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdint.h>
#include "address.h"

typedef volatile unsigned char command;

/* COMMANDS */
#define BIT_SET (command) 0
#define BIT_CLEAR (command) 1
#define BIT_TOGGLE (command) 2
#define REG_SET (command) 3
#define REG_CLEAR (command) 4
#define UART_WRITE (command) 5
#define UART_READ (command) 6
/* COMMANDS END */


/*
	executes a command on the bit/byte in register (at given position) pointed to by given address + offset
	a = base address
	o = address offset
	value = position of the bit, or value to write into register
	returns values if command was a read command
*/
uint32_t exec_command(base_address a, offset o, command c, uint32_t value);

/*
	a = base address
	o = address offset
	p = position of the bit
	returns the value of the bit at given position (p) in the register pointed to by given address
*/
uint8_t getBitValue(address a, offset o, uint32_t p);


#endif
