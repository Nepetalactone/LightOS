/*
 * SchedulerQueue.h
 *
 *  Created on: 12.03.2014
 *      Author: phil
 */

#ifndef SCHEDULERQUEUE_H_
#define SCHEDULERQUEUE_H_

#include "ProcessHeader.h"

/*Structure of every Element of the queue*/
typedef struct Element {
	Process element;
	struct Element *next;
	struct Element *last;
} QueueElement;

/*Creating a new Queue*/
QueueElement* CreateNewQueue(Process firstProcess);
/*Adding a new Element into the queue*/
void Enqueue(QueueElement *queue, Process NewProcess);
/*deleting the first element of the queue (FIFO)*/
QueueElement* Dequeue(QueueElement *queue);
/*get the current size of the queue*/
int getSize();
/*Return a specific Element of the Queue*/
Process getElement(QueueElement *queue, ProcessID PID);
/*Printing the Queue*/
void PrintQueue(QueueElement *queue);//more for testing purpose

#endif /* SCHEDULERQUEUE_H_ */
