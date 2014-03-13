/*
 * ProcessHeader.h
 *
 *  Created on: 12.03.2014
 *      Author: phil
 */

#include <inttypes.h>


#ifndef PROCESSHEADER_H_
#define PROCESSHEADER_H_

typedef void* (*PRFunc)(void*); //Functionsdefintions for the real function of the process
typedef int32_t ProcessID; //Process ID

typedef enum {
	RUNNING, BLOCKED, READY, ZOMBIE
} Processstate;

typedef enum {
	HIGH = 0,
	LOW = 1
}ProcessPriority;

typedef struct {
	Processstate State;
	ProcessID ID;
	PRFunc Process;
	ProcessPriority Priority;
} Process;

#endif /* PROCESSHEADER_H_ */
