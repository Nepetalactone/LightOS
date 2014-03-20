#ifndef ADDRESS_H
#define ADDRESS_H

#include <stdint.h>

#define address(X, Y) __CONCRETE_ADDRESS(X, Y)

//address of a register
typedef volatile uint32_t* address;

//base address
typedef volatile uint32_t base_address;

//offset to base address
typedef volatile uint16_t offset;


address __CONCRETE_ADDRESS(base_address b, offset o);

#endif