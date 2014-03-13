/*
 * Scheduler.h
 *
 *  Created on: 12.03.2014
 *      Author: phil
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "ProcessHeader.h"

ProcessID scheduler_startProcess(PRFunc);
void scheduler_runNextProcess();


#endif /* SCHEDULER_H_ */
