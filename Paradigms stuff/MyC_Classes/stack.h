#include <stdlib.h>
#include <string.h>
#include <assert.h>
const int DEF_SIZE =10;
typedef struct {
    void *elems;
    int elemsize;
    int loglen;
    int alloclen;
    void(*freefn)(void* s);
}stack;

void stackNew(stack *s,int elemsize,void (* freefn)(void *s)){
  s->elemsize=elemsize;
  s->loglen=0;
  s->alloclen = DEF_SIZE;
  s->elems = malloc(s->alloclen*elemsize);
  s->freefn = freefn;
  
}
void stackDispose(stack *s){
  if(s->freefn!= NULL){
    for(int i=0; i< s->loglen; i++){
      s->freefn((char*)s->elems + s->elemsize * i);
    }
  }
  free(s->elems);
}
void stackGrow(stack *s){
  s->alloclen*=2;
  s->elems = realloc(s->elems,s->alloclen*s->elemsize);
  assert(s->elems!=NULL);
}
void stackPush(stack *s, void *elemAddr){
  if(s->loglen == s->alloclen){
    stackGrow(s);
  }
  void * targ = (char*)s->elems+s->loglen*s->elemsize;
  memcpy(targ,elemAddr,s->elemsize);
  s->loglen++;
}
void stackPop(stack* s, void* elemAddr){
  void * source = (char*)s->elems+(s->loglen-1)*(s->elemsize);
  memcpy(elemAddr,source,s->elemsize);
  s->loglen--;
}

