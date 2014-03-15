/*
 * Queue
 *
 *  Created on: 14.03.2014
 *      Author: phil
 */

#ifndef QUEUE_
#define QUEUE_

/*Structure of every Element of the queue*/
typedef struct Element {
	void* element;
	struct Element *next;
	struct Element *last;
} QueueElement;

/*Creating a new Queue*/
QueueElement* CreateNewQueue(void* firstElement);
/*Adding a new Element into the queue*/
void Enqueue(QueueElement *queue, void* NewElement);
/*deleting the first element of the queue (FIFO)*/
QueueElement* Dequeue(QueueElement *queue);
/*get the current size of the queue*/
int getSize();
void PrintQueue(QueueElement *queue);//more for testing purpose


#endif /* QUEUE_ */
