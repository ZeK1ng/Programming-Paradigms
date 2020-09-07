#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>
static const int DEF_ALLOC=50;

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation){
  assert(initialAllocation>=0);
  v->elemsize=elemSize;
  if(initialAllocation ==0){
    v->elems=malloc(DEF_ALLOC*elemSize);
    v->allocLen=DEF_ALLOC;
  }else{
    v->elems = malloc(initialAllocation*elemSize);
    v->allocLen = initialAllocation;
  }
  v->freefn=freeFn;
  v->logLen=0;
 

}


void VectorDispose(vector *v){
  if(v->freefn !=NULL){
    for(int i=0; i< v->logLen; i++){
      v->freefn((char*)v->elems + v->elemsize*i);
    }
  }
  free(v->elems);
  
}

void grow(vector * v){
    void * k = realloc(v->elems,v->allocLen*2*v->elemsize);
    assert(k!=NULL);
    v->elems= k;
    v->allocLen *=2;
}


int VectorLength(const vector *v){
  return v->logLen;
}

void *VectorNth(const vector *v, int position){
  assert(position>=0 && position < v->logLen);
  return (char*)v->elems + v->elemsize*position;
  
}

void VectorReplace(vector *v, const void *elemAddr, int position){
  assert(position>=0 && position < v->logLen);
  if(v->freefn!=NULL)v->freefn((char*)v->elems+v->elemsize*position);
  memcpy((char*)v->elems + v->elemsize*position, elemAddr, v->elemsize);  

}

void VectorInsert(vector *v, const void *elemAddr, int position){
  if(position == v->logLen-1){ 
    VectorAppend(v,elemAddr);
    return;
  }
  if(v->allocLen == v->logLen){
    grow(v);
  }
  
   // move memory from position to postion +1 and copy memory to position from source
  memmove((char*)v->elems+(v->elemsize*(position+1)),
           (char*)v->elems+v->elemsize*position,
           v->elemsize*(v->logLen-position));
  memcpy((char*)v->elems+v->elemsize*position, elemAddr, v->elemsize);  
  
  v->logLen++;

}

void VectorAppend(vector *v, const void *elemAddr){
  if(v->allocLen == v->logLen){
    grow(v);
  }
  memcpy((char*)v->elems+v->elemsize*v->logLen, elemAddr ,v->elemsize);
  v->logLen++; 
  

} 

void VectorDelete(vector *v, int position){
  assert(position>=0 && position < v->logLen);
  if(v->freefn != NULL){
    v->freefn((char*)v->elems+v->elemsize*position);
  }
  
  memmove((char*)v->elems+v->elemsize*position,
          (char*)v->elems+(v->elemsize*(position+1)),
          v->elemsize*(v->logLen-position-1));
  v->logLen--;
  

}

void VectorSort(vector *v, VectorCompareFunction compare){
  assert(compare!=NULL);
  qsort(v->elems,v->logLen,v->elemsize,compare);
  

}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData){
  assert(mapFn!= NULL);
  for(int i=0; i< v->logLen; i++){
    mapFn(VectorNth(v,i),auxData);
  }
  

}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted){
  assert(searchFn != NULL);
  assert(startIndex>=0 && startIndex < v->logLen);
  void * targElem= NULL;
  if(isSorted){
    targElem= bsearch(key,(char*)v->elems + v->elemsize * startIndex,v->logLen-startIndex,v->elemsize,searchFn);
  }else{
    size_t k = v->logLen-startIndex;
    targElem = lfind(key,(char*)v->elems + v->elemsize * startIndex,&k,v->elemsize,searchFn);
  }
  if(targElem == NULL) return kNotFound;
  

  return ((char*)targElem - (char*)v->elems) / v->elemsize;
}

