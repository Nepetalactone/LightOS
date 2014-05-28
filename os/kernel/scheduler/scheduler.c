/*
 * scheduler.c
 * ******* context switch doku**********
 *  http://coactionos.com/embedded%20design%20tips/2013/10/09/Tips-Context-Switching-on-the-Cortex-M3/
 */
#include "process.h"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include "../timer.h"

scheduler_t* scheduler;
queue_t* processes;
static uint16_t process_count = 0;
static unsigned int stack_base;

/*
//Reads the main stack pointer
static inline int rd_stack_ptr(void){
	unsigned int stack_pointer;
	stack_pointer = HWREG("SP");
	//asm ("MOV %0, SP\n\t" : "=r"	(stack_pointer)	);
	return stack_pointer;
}
*/
static process_t* init_idle_process() {
	process_t* idleProcess = (process_t*)malloc(sizeof(process_t));
	idleProcess->procId = 0;
	idleProcess->name = "idle";
	idleProcess->state = READY;
	//stack_base = rd_stack_ptr();
	//uint32_t* pSp = &stack_base;
	idleProcess->sp = (uint32_t*)get_stack_pointer_asm();
	//idleProcess->proc_table->sp =  ;

	++process_count;
	return idleProcess;
}

void init_scheduler(base_address timer) {
	scheduler_t* newScheduler = (scheduler_t*) malloc(sizeof(scheduler_t));
	process_t* idle_proc = init_idle_process();
	newScheduler->curProcess = idle_proc;
	newScheduler->processes = createQueue();
	newScheduler->processes->enqueue(newScheduler->processes, idle_proc);
	newScheduler->timer = timer;
	scheduler = newScheduler;
}

void start_scheduling() {
	run_next_process();
}


void fork(char* procName, void* pc) {
	process_t* proc = malloc(sizeof(process_t));
	proc->name = procName;
	proc->procId = process_count;
	proc->pc = &pc;
	proc->state = READY;
	proc->sp = (uint32_t*)(stack_base + (process_count*256));

	scheduler->processes->enqueue(scheduler->processes, proc);
	++process_count;
}

void context_switch(process_t * nextProcess) {
	//process_t * nextProcess = (process_t *) scheduler->curProcess;
	process_t * curProc = scheduler->curProcess;
	store_context_asm();
	void* sp = (void*) load_context_asm();
}

void run_next_process() {
	queue_t* queue = scheduler->processes;
	printf("test");
	int i;
	int size = queue->size;

	for (i = 0; i < size; i++) {
		process_t* process = (process_t*) queue->dequeue;
		if (process->state == READY) {
			context_switch(process);
			process->state = RUNNING;
			scheduler->curProcess = process;
		} else {
			queue->enqueue(queue, process);
		}
	}

	reset_timer_counter(scheduler->timer);
}
