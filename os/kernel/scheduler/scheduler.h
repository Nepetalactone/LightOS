
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "process.h"
#include "../../utils/queue.h"
#include "../arch/address.h"

#define MAX_PROCESSES 100

typedef void (*proc)(void);

typedef struct scheduler {
	process_t* curProcess;
	queue_t* processes;
	base_address timer;
} scheduler_t;

void fork(char* procName, void* pc);
void run_next_process();
void killProcess(process_t* process);
void init_scheduler(base_address timer);

#endif /* SCHEDULER_H_ */
