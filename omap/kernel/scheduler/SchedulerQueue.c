/*
 * SchedulerQueue.c
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */
#include <stdio.h>
#include <stdlib.h>
#include "SchedulerQueue.h"

Process* getProcess(QueueElement *queue, ProcessID PID) {
	if (queue) {
		if (((Process*)(queue->element))->ID != PID) {
			getProcess(queue->next, PID);
		}else {
			return (Process*) queue->element;
		}
	}
	return NULL;
}

void PrintProcessQueue(QueueElement *queue) {
	if (queue) {
		printf("%d\t", ((Process*)(queue->element))->ID);
		PrintProcessQueue(queue->next);
	} else
		printf("\n");
}

//how to use
//int main(int argc, char** argv) {
//Process* p1;
//	p1->ID = 1;
//
//	Process* p2;
//	p2->ID = 2;
//
//	QueueElement *queue;
//
//	queue = CreateNewQueue(p1);
//
//	Enqueue(queue, p2);
//
//	PrintProcessQueue(queue);
//        PrintQueue(queue);
//
//	queue = Dequeue(queue);
//
//	PrintProcessQueue(queue);
//        PrintQueue(queue);
//
//        Enqueue(queue, p1);
//
//        PrintProcessQueue(queue);
//        PrintQueue(queue);
//
//        queue = Dequeue(queue);
//
//	PrintProcessQueue(queue);
//        PrintQueue(queue);
//
//    return (EXIT_SUCCESS);
//}
