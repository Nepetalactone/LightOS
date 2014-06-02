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

static uint16_t process_count = 0;
static unsigned int stack_base;
static scheduler_t* scheduler;
void run_idle_process(void);
static uint32_t proc_mem_space[] = { 0x82003000, 0x82006000, 0x82009000 };

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

inline void context_switch(process_t* curProc, process_t * nextProc) {
	context_switch2_asm(curProc->pcb.cpsr, nextProc->pcb.cpsr);
}

inline void context_switch_first_time(process_t* curProc, process_t* nextProc){
	context_switch_asm(curProc->pcb.cpsr, nextProc->pcb.cpsr, nextProc->pcb.r14);
}

inline void run_next_process() {

	process_t* curProc = (process_t*) scheduler->curProcess;
	queue_t* queue = scheduler->processes;
	int i;

	for (i = 0; i < queue->size; i++) {
		process_t* nextProc = (process_t*) queue->dequeue(queue);
		if (nextProc->state == READY) {
			nextProc->state = RUNNING;

			scheduler->curProcess->state = READY;

			_disable_interrupts();
			queue->enqueue(queue, nextProc);

			if (nextProc->times_loaded > 0){
				nextProc->times_loaded = nextProc->times_loaded + 1;
				context_switch(curProc, nextProc);
				scheduler->curProcess = nextProc;
				timer_reset_counter(GPTIMER4);
				_enable_interrupts();
			} else {
				nextProc->times_loaded = nextProc->times_loaded + 1;
				context_switch_first_time(curProc, nextProc);
				scheduler->curProcess = nextProc;
				timer_reset_counter(GPTIMER4);
				_enable_interrupts();
				scheduler->curProcess->mainFunc();
			}
			break;
		} else {
			queue->enqueue(queue, nextProc);
		}
	}
}

void init_scheduler(base_address timer) {
	scheduler = (scheduler_t*) malloc(sizeof(scheduler_t));
	scheduler->processes = createQueue();
	create_new_process("init", &run_idle_process);
	scheduler->curProcess = (process_t*) scheduler->processes->dequeue(scheduler->processes);
	scheduler->processes->enqueue(scheduler->processes, scheduler->curProcess);
	scheduler->timer = timer;
	timer_quick_init(timer, 0x05000000, run_next_process,
			trigger_OverflowMatch);
}

void run_idle_process() {
	volatile int i = 0;

	while (1) {
		i = 1;
	}
}

void start_scheduling() {
	_enable_interrupts();
	timer_start(GPTIMER4);
	scheduler->curProcess->state = RUNNING;
	scheduler->curProcess->times_loaded = 1;
	scheduler->curProcess->mainFunc();
}

void create_new_process(char* procName, pFunc asdf) {
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
	proc->state = READY;
	proc->mainFunc = asdf;
	proc->times_loaded = 0;
	scheduler->processes->enqueue(scheduler->processes, proc);
	++process_count;
}
