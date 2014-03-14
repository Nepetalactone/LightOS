#ifndef BINARY_H
#define BINARY_H

#include <stdint.h>

#define binary(x) S_to_binary_(#x)


typedef volatile unsigned int byte;

/*
	turns a string representation of a binary number into TODO change return type?

*/
static inline uint64_t S_to_binary_(const char *s)
{
        uint64_t i = 0;
        while (*s) {
                i <<= 1;
                i += *s++ - '0';
        }
        return i;
}

#endif
