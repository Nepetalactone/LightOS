/*
 * Scheduler.c
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */
#include <stdlib.h>
#include "SchedulerQueue.h"

QueueElement* all_processes;
QueueElement* ready_processes;
Process* current_process;

ProcessID scheduler_startProcess(PRFunc proc){return NULL;}

void scheduler_runNextProcess(){}

/*if the process state is blocked, change it to ready
 * then add the process to the ready process queue*/
void set_to_ready(Process* proc){
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
void set_current_to_blocked(void){
	if(current_process->state == BLOCKED){
		return;
	}
	current_process->state = BLOCKED;
}

void end_process(ProcessID pid, int exit_code){
	Process* p = getProcess(all_processes, pid);
	if (p == NULL) {
		return;
	}

	p->state = ZOMBIE;

	//if needed ipc stuff here
}
