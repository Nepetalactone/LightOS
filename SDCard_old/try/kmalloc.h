/*
 * kmalloc.h
 *
 *  Created on: 11.06.2014
 *      Author: phil
 */

#ifndef KMALLOC_H_
#define KMALLOC_H_


#include <inttypes.h>

void* kmalloc( uint32_t size );
void kfree( void* addr );


#endif /* KMALLOC_H_ */
