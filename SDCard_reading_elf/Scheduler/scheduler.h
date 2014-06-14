/*
 * scheduler.h
 *
 *  Created on: 14.06.2014
 *      Author: phil
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include "../arch/command.h"
#include "../utils/queue.h"
#include "../arch/address.h"
#include "process.h"


#define MAX_PROCESSES 100
#define STACK_SIZE 256


typedef struct scheduler {
	process_t* curProcess;
	queue_t* processes;
	base_address timer;
} scheduler_t;


void init_scheduler();
void start_scheduling();
process_t* process_create(char* name, void* entry_point);
void process_kill_name(char* name);
void process_kill_pid(uint32_t pid);
void run_next_process(void* lr);

#endif /* SCHEDULER_H_ */
