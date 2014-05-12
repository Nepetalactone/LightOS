
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "process.h"
#include "../../utils/queue.h"

#define MAX_PROCESSES 100

typedef struct scheduler {
	process_t* curProcess;
	queue_t* processes;
} scheduler_t;

void addProcess(process_t* process);
void runNextProcess();
void killProcess(process_t* process);


#endif /* SCHEDULER_H_ */
