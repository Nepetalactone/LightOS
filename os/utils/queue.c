#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

//create and initialize queue
queue_t *createQueue () {
    queue_t* queue = (queue_t*) malloc(sizeof(queue_t));
    queue->first = NULL;
    queue->last = NULL;
    queue->size = 0;

    queue->enqueue = &enqueue;
    queue->dequeue = &dequeue;

    return queue;
}

// add element
void enqueue (queue_t* queue, void* element) {
	node_t* node = (node_t*) malloc (sizeof(node_t));
    node->element = element;
    node->next = NULL;

    if (queue->first == NULL) {
        queue->first = node;
    } else{
        queue->last->next = node;
    }
    queue->last = node;
    queue->size++;
}

//remove first element
void* dequeue (queue_t* queue) {
	if (queue->size == 0) {
		return NULL;
	}
    node_t* node = queue->first;
    void* element = node->element;

    queue->first = node->next;
    queue->size = queue->size - 1;

    free(node); //FIXME uncomment
    return element;
}

//return current queue size
uint32_t getSize(queue_t* queue) {
	return queue->size;
}
