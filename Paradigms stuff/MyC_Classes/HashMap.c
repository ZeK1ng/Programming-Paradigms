#include "hashMap.h"
#include <assert.h>
#include <string.h>

HashSetFreeFunction freeFn(void * elem){
    keyVal *curr  = (keyVal *) elem;
    curr->keyFree(curr->key);
    curr->valfree(curr->val);
    free(curr);
}

void HashMapNew(hashMap *hm, int elemSize, int numBuckets,
            HashMapHashFunction hash,
            HashMapKeyCompareFunction keyCompare,
            HashMapKeyFreeFunction keyFree,
            HashMapValueFreeFunction valueFree)
{   
    assert(hm->hash!= NULL);
    assert(hm->keyCmp != NULL);
    assert(hm->keyFree != NULL);
    assert(hm->valueFree!= NULL);
    hm->lSize =elemSize;
    hashset *dataset= malloc(sizeof(hashset));
    HashSetNew(dataset,sizeof(keyVal),numBuckets,hash,keyCompare,freeFn);
}
void HashMapDispose(hashMap *hm){
    HashSetDispose(hm->data);

}
void HashMapAdd(hashMap *hm, const void *key, const void *value){
    keyVal * curr= malloc(sizeof(keyVal)); 
    curr->key = key;
    curr->val = value;
    curr->keyFree=hm->keyFree;
    curr->valfree=hm->valueFree;
    void * found = HashSetLookup(hm->data,curr);
    if(found == NULL){
        HashSetEnter(hm->data,curr);

    }else{
        keyVal* f1 = (keyVal * ) found;
        hm->valueFree(f1->val);
        memcpy(f1->val,value,hm->lSize);
        freeFn(curr);
    }
}