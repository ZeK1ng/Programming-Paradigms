#include "queue.h"

void initQueue(queue* q, size_t elemSize, int(* compFn)(void * v1, void * v2), void(*freeFn )(void * v1)){
	q->loglen = 0;
	q->alloclen = 10;
	q->elemSize = elemSize;
	q->base = malloc(q->alloclen * elemSize);
	q->compFn = compFn;
	q->freeFn =freeFn;
}

void destroyQueue(queue *q ){
	for(int i= 0; i< q->loglen ; i++){
		if(q->freeFn != NULL) q->freeFn(q->base[i].elem);
		free(q->base[i].elem);
	}
	
	free(q->base);
}
void grow(queue * q){
	q->alloclen *= 2;
	q->base = realloc(q->base,sizeof(data)* q->alloclen);
}
void enqueue(queue * q, void * elem){
	if(q->loglen == q->alloclen) grow(q);
	data newElem;
	newElem.elem = malloc(q->elemSize);
	memcpy(newElem.elem,elem,q->elemSize);
	if(q->loglen >0){
		memmove(q->base+1,q->base,sizeof(data)* q->loglen);
	}
	memcpy(q->base,&newElem,sizeof(data));
	q->loglen++;
}

void dequeue(queue * q){
	assert(q->loglen>0);
	
	if(q->freeFn != NULL){
		q->freeFn(q->base[q->loglen-1].elem);
	}
	free(q->base[q->loglen-1].elem);
	q->loglen--;
}

void * front(queue *q ){
	assert(q->loglen>0);
	return q->base[q->loglen-1].elem;
}

int Qsize(queue *q){
	return q->loglen;
}

void * contains(queue *q , void * elem){
	for(int i=0; i< q->loglen ;i++){
		if(q->compFn(q->base[i].elem,elem)== 0){
			return q->base[i].elem;
		}
	}
	return NULL;
}
int  intCmpfn( void * e1,void* e2){
	int n1 = *(int *)e1;
	int n2 = *(int *)e2;
	if(n1 == n2) return 0;
	if(n1>n2) return 1;
	return -1;
}
int StrCmp( void* a,  void* b) {
  return strcmp(*(char**)a, *(char**)b);
}

void StrFree(void* ptr) {
  free(*(char**)ptr);
}
void testStrings(){
	printf("------testing Strings -------\n");
	queue q;
	initQueue(&q,sizeof(char*),StrCmp,StrFree);
  	char *a = strdup("abc"), *b = strdup("xyz");
  	// enqueue(&q,&b);
  	// enqueue(&q,&b);
  	// enqueue(&q,&b);
  	enqueue(&q,&b);

  	// enqueue(&q,&a);
  	// enqueue(&q,&a);
  	// enqueue(&q,&a);
  	enqueue(&q,&a);
  	 printf("AllocLen is %d\n",q.alloclen );
	//enqueue(&q,&g);
	 printf("Size is %d\n",Qsize(&q) );
	 printf("%s\n",*(char ** )front(&q));
	printf("-----deleting elements----\n");
	// dequeue(&q,&a);
	// dequeue(&q,&a);
	// dequeue(&q,&a);
	dequeue(&q);
	 printf("%s\n",*(char ** )front(&q));

	// dequeue(&q,&b);
	// dequeue(&q,&b);
	// dequeue(&q,&b);
	dequeue(&q);
	
	for(int i=0; i< q.loglen; i++){
		printf("elem--%s",*(char**)(q.base[i].elem));
	}
	destroyQueue (&q);

}


void testInts(){
	printf("----Testing Ints----\n");
	queue q;
	initQueue(&q,sizeof(int),intCmpfn,NULL);
	int k =4;
	int m =3;
	enqueue(&q,&k);
	enqueue(&q,&m);

	printf("%d\n",*(int*)q.base[0].elem );
	printf("%d\n",*(int*)front(&q));
	dequeue(&q);
	printf("%d\n",*(int*)front(&q));
	dequeue(&q);
	printf("%d\n",*(int*)front(&q));


	assert(contains(&q,&m)!= NULL);
	/*data curr;
	curr.elem = &k;
	q.base[0]=curr;
	q.loglen++;
	assert(contains(&q,&k)!= NULL);
	printf("%d\n",*(int*)q.base[0].elem );*/
	destroyQueue(&q);


}






