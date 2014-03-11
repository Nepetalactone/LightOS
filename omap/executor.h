#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "address.h"
#include "binary.h"
typedef volatile unsigned char command;

/* COMMANDS */
#define DISABLE (command) 0
#define ENABLE (command) 1
#define SET (command) 2
#define CLEAR (command) 3
#define TOGGLE (command) 4
#define BYTE_SET (command) 5
#define BYTE_CLEAR (command) 6
/* COMMANDS END */


/*
	executes a command on the bit/byte in register (at given position) pointed to by given address + offset
	a = base address
	o = address offset
	p = position of the bit/byte
*/
void bit_operation(address a, offset o, command c,position p);
void byte_operation(address a, offset o, command c, position p, byte b);

/*
	a = base address
	o = address offset
	p = position of the bit
	returns the value of the bit at given position in the register pointed to by given address
*/
unsigned int getBitValue(address a, offset o, position p);




#endif
