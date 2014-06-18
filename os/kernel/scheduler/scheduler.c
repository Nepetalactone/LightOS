
#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "../timer/gptimer.h"
#include "../interrupts/interrupt_controller.h"

void __idle_process();
void __kill(process_t* process);



uint16_t process_count = 0;
//queue_t* process_ready_queue;
//queue_t* process_table;
base_address scheduling_timer = GPTIMER4;

process_t* current_process;
process_t* temp;

process_t* procs[3];
volatile uint32_t counter = 0;
uint32_t times_switched = 0;

void init_scheduler(){
	//process_ready_queue = createQueue();
	//process_table = createQueue();
	//TODO adjust scheduling timer TCRR value
	timer_quick_init(scheduling_timer,0x00200000, run_next_process,trigger_OverflowMatch);
	//times_switched = (uint64_t*)malloc(sizeof(uint64_t));//FIXME REMOVE
	times_switched = 0;
}

void start_scheduling(){
	process_t* process = (process_t*)malloc(sizeof(process_t));
	process_stack_t* stack = (process_stack_t*)malloc(sizeof(process_stack_t));


	process->name = "idle";
	process->pID = process_count;
	process->sp = (void*)stack;
	process->pc = &__idle_process;

	procs[process_count] = process;
	process_count++;

	//process_table->enqueue(process_table,process);
	//process_ready_queue->enqueue(process_ready_queue,process);

	process->state = RUNNING;

	timer_start(scheduling_timer);
	current_process = process;
	__idle_process();
}

void __idle_process(){
	volatile int i = 0;

	while (1) {
		i = 1;
	}
}

process_t* process_create(char* process_name, void* entry_point){
	process_t* process = (process_t*)malloc(sizeof(process_t));
	process_stack_t* stack = (process_stack_t*)malloc(sizeof(process_stack_t));

	process->name = process_name;
	process->pID = process_count;
	process->sp = (void*)stack;
	process->pc = entry_point;
	process->state = READY;

	procs[process_count] = process;
	process_count++;

	process->sp = (void*)init_process_asm(process->sp,process->pc);
	//process_table->enqueue(process_table,process);
	//process_ready_queue->enqueue(process_ready_queue,process);
	return process;
}

void process_kill_name(char* name){

}

void process_kill_pid(uint32_t pid){

}

void __ctx_switch_cleanup();
void __ctx_switch_cleanup(void* sp_old, void* sp_new){
	temp->sp = (void*)sp_old;
	current_process-> sp = (void*)sp_new;
	reset_fiq();
	timer_reset_counter(GPTIMER4);
	_enable_interrupts();
}

void __kill(process_t* process){
	process->state = ZOMBIE;
	//TODO kill process
	//free malloced space
	//remove process from queue(s)
}


void run_next_process(void* lr){

	//if(process_ready_queue->size == 0){
	//	return; //no other ready processes
	//}
	times_switched+=1;
	if(times_switched % 50 == 0){
		//volatile uint64_t asd = *times_switched;
		current_process->state = READY;
	}
	current_process->state = READY;
	//process_ready_queue->enqueue(process_ready_queue,current_process);
	if(counter > 2){
		counter = 0;
	}
	process_t* next_process = procs[counter];
	counter++;
	//process_t* next_process = process_ready_queue->dequeue(process_ready_queue);
	temp = current_process;
	current_process = next_process;
	context_switch_asm(temp->sp,lr, next_process->sp);
}
