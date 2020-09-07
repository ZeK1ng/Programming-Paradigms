#ifndef _hashMap.h_
#define _hashMap.h_
#include "hashset.h"

typedef int (*HashMapHashFunction)(const void *elem, int numBuckets);
typedef int (*HashMapKeyCompareFunction)(const void *elem1, const void* elem2);typedef void (*HashMapKeyFreeFunction)(void *elem);
typedef void (*HashMapValueFreeFunction)(void *elem);
void HashMapNew(hashMap *hm, int elemSize, int numBuckets,
HashMapHashFunction hash,
HashMapKeyCompareFunction keyCompare,
HashMapKeyFreeFunction keyFree,
HashMapValueFreeFunction valueFree);
void HashMapDispose(hashMap *hm);
void HashMapAdd(hashMap *hm, const void *key, const void *value);
typedef struct{
    hashset *  data;
    int lSize;
    HashMapHashFunction hash;
    HashMapKeyCompareFunction keyCmp;
    HashMapKeyFreeFunction keyFree;
    HashMapValueFreeFunction valueFree;
}hashMap;
typedef struct {
    void * key;
    void * val;
    HashMapKeyFreeFunction keyFree;
    HashMapValueFreeFunction valfree;

} keyVal;





#endif