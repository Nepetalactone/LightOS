/*
 * ProcessHeader.h
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */

#ifndef PROCESSHEADER_H_
#define PROCESSHEADER_H_


#include "ipc/message/message.h"

typedef int processID;

typedef enum {
	 RUNNING, READY, BLOCKED, WAIT, ZOMBIE
} processstate; //what kind of states can a thread have

typedef struct {
	processID ID;
	processstate state;
	//PRFunc function;
	void* context;
	uint32_t in_ready_queue;
	struct{
		processID OtherProcess;
		Message* msg;
		QueueElement sender;
	}ipc;
} process_t;


#endif /* PROCESSHEADER_H_ */
