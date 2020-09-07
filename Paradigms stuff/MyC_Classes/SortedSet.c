#include "SortedSet.h"
#include <stdlib.h>
#include <assert.h>
static const int DEFALLOC = 50;
#define NodeSize(elem) sizeof(int)*2+elem

void SetNew(sortedSet * set, int elemSize,int (*cmpfn)(const void *, const void*)){
  assert(cmpfn !=NULL);
  assert(elemSize >0);
  set->root = malloc(sizeof(int ) + DEFALLOC * NodeSize(elemSize));
  assert(set->root != NULL);
  set->cmpfn = cmpfn;
  set->elemsize = elemSize;
  set->loglen= 0;
  set->allocLen= DEFALLOC;
  set->root = -1;
}
bool SetAdd(sortedSet *set,const void * elemPtr){
  
}
void * SetSearch( sortedSet * set, const void * elemPtr){

}