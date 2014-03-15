/*
 * Scheduler.h
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_


ProcessID scheduler_startProcess(PRFunc proc);
void scheduler_runNextProcess();
void set_to_ready(Process* proc);
void set_current_to_blocked(void);
void end_process(ProcessID pid, int exit_code);

#endif /* SCHEDULER_H_ */
