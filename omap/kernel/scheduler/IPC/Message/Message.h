/*
 * Message.h
 * For sending Messages between two Processes (Inter Process Communication)
 *  Created on: 16.03.2014
 *      Author: phil
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>
#include "../../Process/ProcessDefs.h"

#define BUFFERSIZE 1024
#define DATASIZE (BUFFERSIZE/ sizeof( uint32_t ))

typedef struct{
	char buffer[BUFFERSIZE];
	uint32_t data[DATASIZE];
}value_t;

typedef struct{
	ProcessID source;
	ProcessID destination;
	value_t value;
	uint32_t size;
} Message;

#endif /* MESSAGE_H_ */
