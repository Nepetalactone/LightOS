/*
 * address.h
 *
 *  Created on: 14.06.2014
 *      Author: phil
 */

#ifndef ADDRESS_H_
#define ADDRESS_H_


#include <stdint.h>

#define address(BASE, OFF) ((address)(BASE + OFF))

//address of a register
typedef volatile uint32_t* address;

//base address
typedef volatile uint32_t base_address;

//offset to base address
typedef volatile uint16_t offset;

#endif /* ADDRESS_H_ */
