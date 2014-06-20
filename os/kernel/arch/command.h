#ifndef COMMAND_H
#define COMMAND_H

#include "address.h"
#include "../../utils/binary.h"


#define BIT_MASK(x) S_to_binary_(#x)


typedef uint8_t bool;

#define TRUE 1
#define FALSE 0

/*
 *
 * base = base_address
 * off = offset
 * pos = bit position (0 is lowest)
 * val = value to write to register
 *
 */
#define BIT_SET(base,off,pos)			*((address)(base + off))  |= (1 << pos);
#define BIT_CLEAR(base,off,pos)			*((address)(base + off))  &= ~(1 << pos);
#define BIT_TOGGLE(base,off,pos)		*((address)(base + off)) ^= (1 << position);
#define BIT_READ(base,off,pos)			*((address)(base + off)) & (1 << pos);
#define BIT_READ_MASK(base,off,range) 	*((address)(base + off)) &= range

#define REG_SET(base,off,val)		*((address)(base + off)) = val;
#define REG_CLEAR(base,off)			*((address)(base + off)) = 0x0;

/*
 * WFW = WAIT FOR WRITE
 * WFC = WAIT FOR CLEAR
 */
#define WFW_BIT(base,off,pos)		while((*address(base,off) & (1 << pos)) != 1 ){/*WAIT UNTIL BIT IS SET*/}
#define WFC_BIT(base,off,pos)		while(*address(base,off) & (1 << pos)){/*WAIT UNTIL BIT IS CLEARED*/}
#define WFW_REG(base,off,val)		while(*address(base,off) != val){/*WAIT UNTIL REGISTER IS SET*/}
#define WFC_REG(base,off)			while(*address(base,off) != 0){/*WAIT UNTIL REGISTER IS CLEARED*/}

#endif
