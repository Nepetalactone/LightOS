/*
 * ProcessHeader.h
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */

#ifndef PROCESSHEADER_H_
#define PROCESSHEADER_H_


#include "Process/ProcessDefs.h"
#include "SchedulerQueue.h"
#include "IPC/Message/Message.h"

typedef enum {
	 RUNNING, READY, BLOCKED, WAIT, ZOMBIE
} Processstate; //what kind of states can a thread have

typedef struct {
	ProcessID ID;
	Processstate state;
	//PRFunc function;
	void* context;
	uint32_t in_ready_queue;
	struct{
		ProcessID OtherProcess;
		Message* msg;
		QueueElement sender;
	}ipc;
} Process;

#endif /* PROCESSHEADER_H_ */
