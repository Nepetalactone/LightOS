#ifndef BINARY_H
#define BINARY_H

#include <stdint.h>
#include "../kernel/arch/address.h"

#define binary(x) S_to_binary_(#x)


typedef volatile unsigned int byte;

/*
	turns a string representation of a binary number into

*/
static inline uint64_t S_to_binary_(const char *s);

unsigned int getBitValue(address a, offset o, position p);

#endif
