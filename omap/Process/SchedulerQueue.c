/*
 * SchedulerQueue.c
 *
 *  Created on: 12.03.2014
 *      Author: phil
 */
#include <stdio.h>
#include <stdlib.h>
#include "SchedulerQueue.h"

static int queuesize = 0;

QueueElement* CreateNewQueue(Process firstProcess) {
	QueueElement *newQueue;
	newQueue = (QueueElement *) malloc(sizeof(QueueElement));
	newQueue->next = NULL;
	newQueue->element = firstProcess;
	newQueue->last = newQueue;

	queuesize = 0;

	return newQueue;
}

void Enqueue(QueueElement *queue, Process NewProcess) {
	if (queue)/*if the queueelement is not empty it should go inside*/{
		Enqueue(queue->next, NewProcess);
		if (!queue->next)/*if the next queueelement is empty*/{
			queue->next = (QueueElement *) malloc(sizeof(QueueElement));
			queue->next->element = NewProcess;
			queue->next->last = NULL;
			queuesize++;
		}
	}
}

QueueElement* Dequeue(QueueElement *queue) {
	if (queue->last) {
		queue = queue->next;
		queue->last = queue->next;
		return queue;
	}
}

int getSize() {
	return queuesize;
}

Process getElement(QueueElement *queue, ProcessID PID) {
	if (queue) {
		if (queue->element.ID != PID) {//TODO: test
			getElement(queue->next, PID);
		}else {
			return queue->element;
		}
	}
}

void PrintQueue(QueueElement *queue) {
	if (queue) {
		printf("%d\t", queue->element.ID);
		PrintQueue(queue->next);
	} else
		printf("\n");
}

/*int main(void){
	Process p1;
	p1.ID = 1;

	Process p2;
	p2.ID = 2;

	QueueElement *queue;

	queue = CreateNewQueue(p1);

	Enqueue(queue, p2);

	PrintQueue(queue);

	queue = Dequeue(queue);

	PrintQueue(queue);

        Enqueue(queue, p1);

        PrintQueue(queue);

        queue = Dequeue(queue);

	PrintQueue(queue);
    return (EXIT_SUCCESS);
}*/
