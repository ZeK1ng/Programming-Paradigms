#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


typedef void (*DqFreeFn)(void * elem);
typedef int(*DqCompareFn)(void * v1, void * v2);

typedef struct 
{
	void * elem;
}data;

typedef struct 
{
	data * base;
	int logLen;
	int allocLen;
	size_t elemSize;
	DqFreeFn freefn;
	DqCompareFn compFn;
}deque;
void InitDeq(deque * dq,size_t elemSize,DqFreeFn freefn,DqCompareFn compFn){
	dq->logLen =0;
	dq->allocLen = 10;
	dq->elemSize = elemSize;
	dq->freefn = freefn;
	dq->compFn = compFn;
	dq->base = malloc(sizeof(data) * dq->allocLen);
}
void DqDestroy(deque * dq){
	if(dq->freefn != NULL){
		for(int i=0; i< dq->logLen; i++){
			dq->freefn(dq->base[i].elem);
		}
	}
	free(dq->base);
}
void checkAndGrow(deque * dq){
	if(dq->logLen == dq->allocLen) {
		dq->allocLen *=2;
		dq->base = realloc(dq->base,sizeof(data)*dq->allocLen );
	}
}
//Add element at the end (public member function )
void push_back(deque * dq,void * elem){
	checkAndGrow(dq);
	data newData;
	newData.elem = malloc(dq->elemSize);
	memcpy(newData.elem,elem,dq->elemSize);
	memcpy(dq->base+dq->logLen,&newData,sizeof(data));
	//free(newData.elem);
	dq->logLen++;
}

//Insert element at beginning (public member function )
void push_front(deque * dq, void * elem){
	checkAndGrow(dq);
	data newData;
	newData.elem = malloc(dq->elemSize);
	memcpy(newData.elem,elem,dq->elemSize);
	memmove(dq->base+1,dq->base,dq->logLen * sizeof(data));
	memcpy(dq->base,&newData,sizeof(data));
	dq->logLen++;
}
void * getFront(deque *dq){
	assert(dq->logLen != 0);

	return dq->base[0].elem;
}
void * getBack(deque * dq){
	assert(dq->logLen != 0);

	return dq->base[dq->logLen-1].elem;
}
//Delete last element (public member function )
void pop_back(deque * dq){
	assert(dq->logLen != 0);
	if(dq-> freefn != NULL){
		dq->freefn(dq->base[dq->logLen].elem);
	}
	dq->logLen--;
}
//Delete first element (public member function )
void pop_front(deque *dq){
	assert(dq->logLen != 0);
	if(dq-> freefn != NULL){
		dq->freefn(dq->base[0].elem);
	}
	dq->logLen--;
	memmove(dq->base,dq->base+1,dq->logLen*sizeof(data));
}

//Insert element at a given index (public member function )
void insert(deque * dq, int index, void * elem){
	checkAndGrow(dq);
	memmove(dq->base+index+1,dq->base+index,(dq->logLen-index)*sizeof(data));
	data newData;
	newData.elem = malloc(dq->elemSize);
	memcpy(newData.elem,elem,dq->elemSize);
	memcpy(dq->base+index,&newData,sizeof(data));
	dq->logLen++;
}

int main(){
	deque dq;
	InitDeq(&dq,sizeof(int),NULL,NULL);
	int k =4;
	int m =5;
	push_back(&dq,&k);
	push_front(&dq,&m);
	push_front(&dq,&m);
	printf("%d\n",*(int*)(getBack(&dq)));
	printf("%d\n",*(int*)(getFront(&dq)));
	int j = 10;
	insert(&dq,3,&j);
	printf("%d\n",*(int*)(getBack(&dq)));
	DqDestroy(&dq);

	return 0 ;
}