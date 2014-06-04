
#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "../timer/gptimer.h"
#include "../interrupts/interrupt_controller.h"

void __idle_process();
void __kill(process_t* process);



uint16_t process_count = 0;
queue_t* process_ready_queue;
queue_t* process_table;
base_address scheduling_timer = GPTIMER4;

process_t* current_process;


void init_scheduler(){
	process_ready_queue = createQueue();
	process_table = createQueue();
	//TODO adjust scheduling timer TCRR value
	timer_quick_init(scheduling_timer,0x02000000, run_next_process,trigger_OverflowMatch);

}

void start_scheduling(){
	process_t* idle_process = process_create("idle",  &__idle_process);
	idle_process->state = RUNNING;

	timer_start(scheduling_timer);
	current_process = idle_process;
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


	process_count++;

	init_process_asm(process->sp,process->pc);
	process_table->enqueue(process_table,process);
	process_ready_queue->enqueue(process_ready_queue,process);
	return process;
}

void process_kill_name(char* name){

}

void process_kill_pid(uint32_t pid){

}

void __ctx_switch_cleanup();
void __ctx_switch_cleanup(){
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

	if(process_ready_queue->size == 0){
		return; //no other ready processes
	}

	current_process->state = READY;
	process_ready_queue->enqueue(process_ready_queue,current_process);

	process_t* next_process = process_ready_queue->dequeue(process_ready_queue);
	process_t* temp = current_process;
	current_process = next_process;
	context_switch_asm(temp->sp,lr, next_process->sp);
}
