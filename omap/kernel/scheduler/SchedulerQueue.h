/*
 * SchedulerQueue.h
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */

#ifndef SCHEDULERQUEUE_H_
#define SCHEDULERQUEUE_H_

#include "../../utils/queue/Queue.h"
#include "ProcessHeader.h"

/*Return a specific Element of the Queue*/
Process* getProcess(QueueElement *queue, ProcessID PID) ;
/*Printing the Queue*/
void PrintProcessQueue(QueueElement *queue);//more for testing purpose


#endif /* SCHEDULERQUEUE_H_ */
