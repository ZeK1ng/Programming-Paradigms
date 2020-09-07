#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
	assert(elemSize >0);
	assert(numBuckets>0);
	assert(comparefn != NULL);
	assert(hashfn != NULL);
	h->data = malloc(sizeof(vector*) * numBuckets);
	h->elemsize=elemSize;
	h->compFn=comparefn;
	h->freeFn=freefn;
	h->hashFn=hashfn;
	h->numBuckets = numBuckets;
	h->loglen = 0;
	for(int i=0; i<numBuckets; i++){
		vector * newV = malloc(sizeof(vector));
		VectorNew(newV,elemSize,h->freeFn,0);
		h->data[i]=newV;
	}
}

void HashSetDispose(hashset *h){
	for(int i =0; i< h->numBuckets; i++){
		VectorDispose(h->data[i]);
		free(h->data[i]);
	}
	free(h->data);	
}

int HashSetCount(const hashset *h){
	return h->loglen;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData){
	assert(mapfn != NULL);
	for(int i=0;i<h->numBuckets; i++){
		VectorMap(h->data[i],mapfn,auxData);
	}
}
void HashSetEnter(hashset *h, const void *elemAddr){
	assert(elemAddr != NULL);
	int buck = h->hashFn(elemAddr,h->numBuckets);
	assert(buck>=0 && buck<h->numBuckets);
	int addr = VectorSearch(h->data[buck],elemAddr,h->compFn,0,true);
	if(addr == -1){
		VectorAppend(h->data[buck],elemAddr);
		VectorSort(h->data[buck],h->compFn);
		h->loglen++;
	}else{
		VectorReplace(h->data[buck],elemAddr,addr);
	}
}

void *HashSetLookup(const hashset *h, const void *elemAddr){
	assert(elemAddr != NULL);
	int buck = h->hashFn(elemAddr,h->numBuckets);
	assert(buck>=0 && buck < h->numBuckets);
	int addr = VectorSearch(h->data[buck],elemAddr,h->compFn,0,true);
	if(addr == -1) return NULL;
	return VectorNth(h->data[buck],addr);	
}
