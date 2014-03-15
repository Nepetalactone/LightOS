/*
 * Queue.c
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */

#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"


static int queuesize = 0;

QueueElement* CreateNewQueue(void* firstProcess) {
	QueueElement *newQueue;
	newQueue = (QueueElement *) malloc(sizeof(QueueElement));
	newQueue->next = NULL;
	newQueue->element = firstProcess;
	newQueue->last = newQueue;

	queuesize = 0;

	return newQueue;
}

void Enqueue(QueueElement *queue, void* NewProcess) {
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
	return NULL;
}

int getSize() {
	return queuesize;
}

void PrintQueue(QueueElement *queue) {
	if (queue) {
		printf("%d\t", queue->element);
		PrintQueue(queue->next);
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


