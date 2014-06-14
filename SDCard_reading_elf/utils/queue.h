/*
 * queue.h
 *
 *  Created on: 27.04.2014
 *      Author: Chris
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "stdint.h"

typedef struct node {
	void* element;
	struct node *next;
} node_t;

/*Structure of the queue*/
typedef struct queue {
	uint32_t size;
	node_t *first;
	node_t *last;

	void (*enqueue)(struct queue* queue, void* element);
	void* (*dequeue)(struct queue* queue);
	//void (*printQueue)(struct queue_t* queue);
} queue_t;


/*Adding a new Element into the queue*/
void enqueue(queue_t *queue, void* newElement);

/*deleting the first element of the queue (FIFO)*/
void* dequeue (queue_t* queue);

/*get the current size of the queue*/
uint32_t getSize(queue_t* queue);

//create new queue
queue_t *createQueue();


//void printQueue(queue_t *queue);//more for testing purpose

#endif /* QUEUE_H_ */
