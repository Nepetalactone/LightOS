
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "process.h"
#include "../../utils/queue.h"

#define MAX_PROCESSES 100

typedef struct scheduler {
	process_t* curProcess;
	queue_t* processes;
} scheduler_t;

void fork(char* procName, uint32_t *pc);
void run_next_process();
void killProcess(process_t* process);


#endif /* SCHEDULER_H_ */
