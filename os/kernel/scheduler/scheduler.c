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
static uint16_t process_count;

static process_t* get_idle_process() {
	process_t* idleProcess = malloc(sizeof(process_t));
	idleProcess->procId = 0;
	idleProcess->name = "idle";
	idleProcess->state = READY;
	return idleProcess;
}


void initScheduler() {
	scheduler_t* newScheduler = (scheduler_t*) malloc(sizeof(scheduler_t));
	process_t* idle_proc = get_idle_process();
	newScheduler->curProcess = idle_proc;
	newScheduler->processes = createQueue();
	newScheduler->processes->enqueue(newScheduler->processes, idle_proc);
	scheduler = newScheduler;
}



void fork(char* procName, uint32_t *pc) {
	process_t* proc;
	proc->name = procName;
	proc->pc = pc;
	proc->state = READY;
	//asm("NOP");
	//asm ()
	scheduler->processes->enqueue(scheduler->processes, proc);
}



//Reads the main stack pointer
static inline int rd_stack_ptr(void){
	unsigned int stack_pointer;
	stack_pointer = HWREG("SP");
	//asm ("MOV %0, SP\n\t" : "=r"	(stack_pointer)	);
	return stack_pointer;
}

/*
//This saves the context on the PSP, the Cortex-M3 pushes the other registers using hardware
static inline void save_context(void){
  uint32_t scratch;
  asm ("MRS %0, psp\n\t STMDB %0!, {r4-r11}\n\t MSR psp, %0\n\t"  : "=r" (scratch) );
}


//This loads the context from the PSP, the Cortex-M3 loads the other registers using hardware
static inline void load_context(void){
  uint32_t scratch;
  asm ("MRS %0, psp\n\t LDMFD %0!, {r4-r11}\n\t MSR psp, %0\n\t"  : "=r" (scratch) );
}
*/



void context_switch(process_t * nextProcess) {
	process_t * curProcess = (process_t *) scheduler->curProcess;
	store_context_asm();
	load_context_asm();



}




void run_next_process() {
	queue_t* queue = scheduler->processes;
	int i;

	for (i = 0; i < queue->size; i++) {
		process_t* process = (process_t*) queue->dequeue;
		if (process->state == READY) {
			context_switch(process);
			process->state = RUNNING;
			scheduler->curProcess = process;


		} else {
			queue->enqueue(queue, process);
		}
	}

	//reset_timer();
}
