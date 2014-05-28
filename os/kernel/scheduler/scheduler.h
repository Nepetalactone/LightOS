
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "process.h"
#include "../../utils/queue.h"
#include "../arch/address.h"

extern unsigned int SPSR;
extern unsigned int SPSR_fsxc;

#define MAX_PROCESSES 100
#define STACK_SIZE 256


typedef struct scheduler {
	process_t* curProcess;
	queue_t* processes;
	base_address timer;
} scheduler_t;

void start_scheduling(void);
void create_new_process(char* procName, pFunc asdf);
void run_next_process(void);
void killProcess(process_t* process);
void init_scheduler(base_address timer);

#endif /* SCHEDULER_H_ */
