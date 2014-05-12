/*
 * Scheduler.c
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */
#include <stdlib.h>
#include "process.h"

static process_t* curProcess;
static uint32_t nextProcessId;


void Scheduler_init(){
	nextProcessId = 0;
	//mmu und timer starten
}

processID scheduler_startProcess(unsigned int virtual_address, unsigned int physical_address) {
	process_t* process = malloc(sizeof(process_t));

	if (process == NULL) {
		return NULL;
	}

	//mmu and ipc stuff
	process->state = READY;
	process->ID = nextProcessId++;
	//process->context = proc;
	//Enqueue(all_processes, process);
	//Enqueue(ready_processes, process);
	return process->ID;
}

process_t* scheduler_runNextProcess() {

	while (ready_processes != NULL) {
		QueueElement node = Dequeue(&ready_processes);
		if (node != NULL) {
			process_t* p = node->element;
			if (p->state == READY || p->state == RUNNING) {
				Enqueue(&ready_processes, node);
				return node->element;
			} else {
				p->in_ready_queue = 0;
				free(node);
			}
		}
	}
	return NULL;
}

/*if the process state is blocked, change it to ready
 * then add the process to the ready process queue*/
void set_to_ready(process_t* proc) {
	if (proc->state != BLOCKED) {
		return;
	}
	proc->state = READY;
	if (proc->in_ready_queue == 0) {
		Enqueue(ready_processes, proc);
		proc->in_ready_queue = 1;
	}

}

/*checking if the current process is not blocked,
 *  and set it to blocked*/
void set_current_to_blocked(void) {
	if (curProcess->state == BLOCKED) {
		return;
	}
	curProcess->state = BLOCKED;
}

void end_process(ProcessID pid, int exit_code) {
	process_t* p = getProcess(all_processes, pid);
	if (p == NULL) {
		return;
	}

	p->state = ZOMBIE;

	//if needed ipc stuff here
}
