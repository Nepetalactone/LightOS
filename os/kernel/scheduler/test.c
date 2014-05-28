
#include <stdint.h>
#include <stdlib.h>
#include "test.h"
#include "../timer/gptimer.h"
#include "../interrupts/interrupt_controller.h"

void __idle_process();
void __kill(process_t* process);



uint16_t process_count = 0;
uint32_t proc_mem_space[3] = {0x82003000, 0x82006000, 0x82009000};
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
	process_t* idle_process = (process_t*)malloc(sizeof(process_t));
	idle_process->pID = process_count;
	idle_process->name = "idle";
	idle_process->state = READY;
	idle_process->pc = &__idle_process;

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

void process_create(char* process_name, void* entry_point){
	process_t* process = malloc(sizeof(process_t));
	process->name = process_name;
	process->pID = process_count;
	process->pc = entry_point;
	process->state = READY;

	process_count++;
	process_table->enqueue(process_ready_queue,process);
}

void process_kill_name(char* name){

}

void process_kill_pid(uint32_t pid){

}

void __kill(process_t* process){
	process->state = ZOMBIE;
	//TODO kill process
	//remove process from queue(s)
}

void run_next_process(){

	if(process_ready_queue->size == 0){
		return; //no other ready processes
	}

	current_process->state = READY;
	process_ready_queue->enqueue(process_ready_queue,current_process);

	process_t* next_process = process_ready_queue->dequeue(process_ready_queue);

	context_switch_asm(current_process, next_process);
}
