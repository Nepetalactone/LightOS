/*
 * common.h
 *
 *  Created on: 11.06.2014
 *      Author: phil
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <inttypes.h>

typedef uint8_t bool;

#define TRUE 1
#define FALSE 0

#define READ_REGISTER( addr ) (*(volatile uint32_t *)( addr ))
#define READ_REGISTER_OFFSET( base, offset ) (*(volatile uint32_t *)((base)+(offset)))

#define BIT_SET( val, bits ) ( val |= bits )
#define BIT_CLEAR( val, bits ) ( val &= ~(bits) )
#define BIT_KEEP( val, bits ) ( val &= bits )
#define BIT_CLEAR_ALL( val ) ( val = 0x0 )
#define BIT_CHECK( val, bits ) ( ( val & bits ) == bits )

#endif /* COMMON_H_ */
