#include <stdio.h>
#include "vector.h"
#include "hashset.h"
#include <stdlib.h>
#include <string.h>

typedef int (*MultiTableHashFunction)(const void * keyAddr, int numbuckets);
typedef int(*MultiTableCompareFunction)(const void *keyAddr1,const void * keyAddr2);
typedef void (*MultitableMapFunction)(void * keyAddr,void * valueAddr, void * auxData);

typedef struct 
{ 
  hashset mappings;
  int keySize;
  int valueSize;
}multiTable;

void MultiTableNew(multiTable * mt,int keySizeInBytes,int valueSizeInBytes,int numBucks, 
                        MultiTableHashFunction hash,MultiTableCompareFunction compare)
                        
{
  mt->keySize = keySizeInBytes;
  mt->valueSize = valueSizeInBytes;
  HashSetNew(&mt->mappings,keySizeInBytes+sizeof(vector),numBucks,hash,compare,NULL);

}
void MultiTableEnter(multiTable *mt,const void *keyAddr,const void * valueAddr){
  void * findKey = HashSetLookup(&mt->mappings,keyAddr);
  vector * currVec;
  char data [mt->keySize + sizeof(vector)];
  if(findKey == NULL){
    memcpy(data,keyAddr,mt->keySize);
    currVec = (vector*)(data+mt->keySize);
    VectorNew(currVec,mt->valueSize,NULL,0);
    VectorAppend(currVec,valueAddr);
    HashSetEnter(mt->mappings,data);
  }else{
    currVec= (vector *)((char*)find + mt->keySize);
    VectorAppend(currVec,valueAddr);
  }

}
void MultiTableMap(multiTable * mt,MultitableMapFunction map, void * auxData){

}

