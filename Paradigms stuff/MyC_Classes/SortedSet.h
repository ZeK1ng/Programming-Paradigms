#include <stdbool.h>
#ifndef _SORTEDSET_
#define _SORTEDSET_

typedef struct{
  int * root;
  int elemsize;
  int loglen;
  int allocLen;
  int (*cmpfn)(const void *, const void *);
}sortedSet;

void SetNew(sortedSet * set, int elemSize,int (*cmpfn)(const void *, const void*));
bool SetAdd(sortedSet *set,const void 8 elemPtr);
void * SetSearch( sortedSet * set, const void * elemPtr);
#endif