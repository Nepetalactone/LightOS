#ifndef ADDRESS_H
#define ADDRESS_H

#include <stdint.h>
//address of a register
typedef volatile uint32_t *address;

//base address
typedef volatile unsigned char* base_address;

//offset to base address
typedef volatile unsigned int offset;

//bit/byte position in a register
typedef volatile unsigned int position;

#endif
