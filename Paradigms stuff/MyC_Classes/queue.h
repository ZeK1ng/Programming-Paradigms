#ifndef __QUEUEUEEUE___
#define __QUEUEUEEUE___
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct 
{
	void * elem;
}data;

typedef struct 
{
	data * base;
	int loglen;
	int alloclen;
	size_t elemSize;
	int(* compFn)(void * v1, void * v2);
	void(*freeFn)(void * v1);

} queue;


void initQueue(queue* q, size_t elemSize, int(* compFn)(void * v1, void * v2), void(*freeFn )(void * v1));
void destroyQueue(queue *q );
void grow(queue * q);
void enqueue(queue * q, void * elem);
void dequeue(queue * q);
void * front(queue *q );
int Qsize(queue *q);
void * contains(queue *q , void * elem);

#endif
