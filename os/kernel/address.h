#ifndef ADDRESS_H
#define ADDRESS_H

#include <stdint.h>

#define address(BASE, OFF) ((address)(BASE + OFF))

//address of a register
typedef volatile uint32_t* address;

//base address
typedef volatile uint32_t base_address;

//offset to base address
typedef volatile uint16_t offset;


#endif
