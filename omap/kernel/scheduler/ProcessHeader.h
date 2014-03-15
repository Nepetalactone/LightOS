/*
 * ProcessHeader.h
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */

#ifndef PROCESSHEADER_H_
#define PROCESSHEADER_H_

#include <stdint.h>

typedef void* (*PRFunc)(void*); //Functionsdefintions for the real function of the process
typedef char ProcessID; //Process ID

typedef enum {
	 RUNNING, READY, BLOCKED, WAIT, ZOMBIE
} Processstate; //what kind of states can a thread have

typedef struct {
	ProcessID ID;
	Processstate state;
	PRFunc function;
	uint32_t in_ready_queue;
} Process;

#endif /* PROCESSHEADER_H_ */
