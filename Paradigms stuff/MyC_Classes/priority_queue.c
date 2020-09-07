#ifndef _PRIORITYY_QUEUEE
#define _PRIORITYY_QUEUEE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

typedef  int(*PQcompFn)(const void * v1, const void * v2);
typedef void (*  PQfreeFn)(void * v1);
typedef struct
{
	int priority;
	void * elem;
}data;

typedef struct 
{
	data * base;
	int alloclen;
	int loglen;
	size_t elemSize;
	PQcompFn compfn;
	PQfreeFn freefn;
}pq;

void initPq(pq * q, size_t elemSize,PQcompFn compf,PQfreeFn freefn){
	q->alloclen = 10;
	q->loglen =0;
	q->elemSize =elemSize;
	q->base = malloc(q->alloclen * sizeof(data));
	q->compfn=compf;
	q->freefn=freefn;
}

void PqDestroy(pq *q){
	if(q->freefn != NULL){
		for(int i=0; i< q->loglen; i++){
			q->freefn(q->base[i].elem);
		}
	}
	free(q->base);
}

void pqInsert(pq * q, void * elem, int priority){
	if(q->loglen == q->alloclen){
		q->alloclen *= 2;
		q->base = realloc(q->base, q->alloclen * sizeof(data));
	}
	data newData;
	newData.priority = priority;
	newData.elem = malloc(q->elemSize);
	memcpy(newData.elem,elem,q->elemSize);
	memcpy(q->base+q->loglen,&newData,sizeof(data));
	q->loglen++;
}
int getMaxIndex(pq * q){
	int index = 0;
	int currPriority = INT_MIN;
	for(int i=0; i< q->loglen; i++){
		if(q->base[i].priority>=currPriority){
			currPriority = q->base[i].priority;
			index= i;
		}
	}
	return index;
}
void * getHighestPrElem(pq *q ){
	assert(q->loglen > 0);
	int index = getMaxIndex(q);
	return q->base[index].elem;
}
void deleteHighestPrElem(pq *q ){
	assert(q->loglen > 0);
	int index = getMaxIndex(q);
	if(q->freefn != NULL){
		q->freefn(q->base[index].elem);
	}
	memmove(q->base+index,q->base+index+1,(q->loglen-index-1)*sizeof(data));
	q->loglen--;

}

int getSize(pq *q){
	return q->loglen;
}
void testInts(){
	pq q;
	initPq(&q,sizeof(int),NULL,NULL);
	int k = 4;
	int j = 10;
	int as= 120;
	pqInsert(&q,&k,100);
	pqInsert(&q,&j,20);
	pqInsert(&q,&as,101);
	printf("%d--%d--%d\n",q.loglen,q.base[0].priority,*(int *)q.base[0].elem );
	printf("%d\n",*(int *)getHighestPrElem(&q));
	deleteHighestPrElem(&q);
	printf("%d\n",*(int *)getHighestPrElem(&q));
	deleteHighestPrElem(&q);
	printf("%d\n",*(int *)getHighestPrElem(&q));
	// deleteHighestPrElem(&q);
	// printf("%d\n",*(int *)getHighestPrElem(&q));
	PqDestroy(&q);

}
int main(){
	testInts();
	return 0;
}





#endif