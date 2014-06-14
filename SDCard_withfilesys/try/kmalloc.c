/*
 * kmalloc.c
 *
 *  Created on: 11.06.2014
 *      Author: phil
 */

#include "kmalloc.h"

#include <stdlib.h>

// module-local defines

#define MEMORY_POOL_SIZE 0x1000000
////////////////////////////////////////////////

// module-local data-structures

////////////////////////////////////////////////

// module-local functions

////////////////////////////////////////////////

// module-local data

//static uint8_t _memoryPool[ MEMORY_POOL_SIZE ];
////////////////////////////////////////////////

void*
kmalloc( uint32_t size )
{
return malloc( size );
}

void
kfree( void* addr )
{
free( addr );
}
