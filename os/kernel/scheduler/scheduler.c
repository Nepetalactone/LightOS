/*
 * scheduler.c
 * ******* context switch doku**********
 *  http://coactionos.com/embedded%20design%20tips/2013/10/09/Tips-Context-Switching-on-the-Cortex-M3/
 */
#include "process.h"
#include "scheduler.h"
#include "../timer/gptimer.h"
#include <stdio.h>
#include <stdlib.h>


//static queue_t* processes;
static uint16_t process_count = 0;
static unsigned int stack_base;
static scheduler_t* scheduler;
void run_idle_process(void);
static uint32_t proc_mem_space[] = {0x82003000, 0x82006000, 0x82009000};

static void switch_user_mode() {
	asm("CPS	#16");
}

static void switch_system_mode() {
	asm("CPS	#0x1f");
}


/*
//Reads the main stack pointer
static inline int rd_stack_ptr(void){
	unsigned int stack_pointer;
	//stack_pointer = HWREG("SP");
	__asm__ volatile ("MOV %0, SP\n\t" : "=r"	(stack_pointer)	);
	return stack_pointer;
}*/


inline void context_switch(process_t* curProc , process_t * nextProc) {
	if (nextProc->times_loaded > 1) {

		context_switch2_asm(curProc->pcb.cpsr, nextProc->pcb.cpsr);
	} else {
		context_switch_asm(curProc->pcb.cpsr, nextProc->pcb.cpsr, nextProc->pcb.r14); //, nextProc->pcb.r14
	}

	//store_context_asm(curProc->pcb.cpsr);
	//load_context_asm(nextProc->pcb.cpsr, nextProc->pcb.r14);
	//
	//curProc->state = READY;
}

inline void run_next_process() {
	process_t* curProc = (process_t*) scheduler->curProcess;
	queue_t* queue = scheduler->processes;
	int i;
	int size = queue->size;

	for (i = 0; i < size; i++) {
		process_t* nextProc = (process_t*) queue->dequeue(queue);
		if (nextProc->state == READY) {
			nextProc->times_loaded = nextProc->times_loaded + 1;
			nextProc->state = RUNNING;


			queue->enqueue(queue, nextProc);
			timer_reset_counter(GPTIMER4);
			_enable_interrupts();
			context_switch(curProc, nextProc);
			scheduler->curProcess = nextProc;
			break;
		} else {
			queue->enqueue(queue, nextProc);
		}
	}

}

static process_t* init_idle_process() {
	process_t* idleProcess = (process_t*)malloc(sizeof(process_t));
	idleProcess->pID = process_count;
	idleProcess->name = "idle";
	idleProcess->state = READY;
	idleProcess->times_loaded = 0;

	//idleProcess->sp = (unsigned int*) stack_base;
	idleProcess->pc = run_idle_process;

	idleProcess->pcb.cpsr = proc_mem_space[process_count];

	++process_count;
	return idleProcess;
}

void init_scheduler(base_address timer) {
	scheduler_t* newScheduler = (scheduler_t*) malloc(sizeof(scheduler_t));
	process_t* idle_proc = init_idle_process();
	newScheduler->curProcess = idle_proc;
	newScheduler->processes = createQueue();
	//newScheduler->processes->enqueue(newScheduler->processes, idle_proc);
	newScheduler->timer = timer;
	//init scheduling timer
	timer_quick_init(timer,0x05000000, run_next_process,trigger_OverflowMatch);
	scheduler = newScheduler;
}


void run_idle_process(){
	volatile int i = 0;

	while (1) {
		i = 1;
	}
}

void start_scheduling() {
	//run_next_process();
	_enable_interrupts();
	timer_start(GPTIMER4);

	run_idle_process();
}


void fork(char* procName, pFunc asdf) {
	process_t* proc = malloc(sizeof(process_t));
	proc->name = procName;
	proc->pID = process_count;

	proc->pcb.cpsr = proc_mem_space[process_count];
	proc->pcb.r14 = (uint32_t) asdf;
	proc->pcb.r13 = (uint32_t) asdf;
	proc->pcb.r12 = (uint32_t) asdf;
	proc->pcb.r11 = (uint32_t) asdf;
	proc->pcb.r10 = (uint32_t) asdf;
	proc->pcb.r9 = (uint32_t) asdf;
	proc->pcb.r8 = (uint32_t) asdf;
	proc->pcb.r7 = (uint32_t) asdf;
	proc->pcb.r6 = (uint32_t) asdf;
	proc->pcb.r5 = (uint32_t) asdf;
	proc->pcb.r4 = (uint32_t) asdf;
	proc->pcb.r3 = (uint32_t) asdf;
	proc->pcb.r2 = (uint32_t) asdf;
	proc->pcb.r1 = (uint32_t) asdf;
	proc->pcb.r0 = (uint32_t) asdf;


	printf("%p", asdf);
	//proc->pc = &pc;
	proc->state = READY;
	//proc->sp = (unsigned int*)(stack_base + (process_count*STACK_SIZE));
	proc->times_loaded = 0;
	scheduler->processes->enqueue(scheduler->processes, proc);
	++process_count;
}
