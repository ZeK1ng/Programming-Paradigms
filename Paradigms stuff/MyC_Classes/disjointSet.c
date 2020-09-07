#include "disjointSet.h"

VectorFreeFunction vecFree(vector * v){
    for(int i=0; i<VectorLength(v); i++){
        HashSetDispose(v[i]);
    }
    free(v);
}
void DisjointSetsNew(disjointsets *ds){
    VectorNew(ds->vec,sizeof(hashset),vecFree,0);
}

int haveUnion(hashset *s1,hashset* s2){
    
}

void DisjointSetsAdd(disjointsets *ds, hashset *s){
    for(int i=0; i<VectorLength(ds->vec); i++){
        hashset * curr = VectorNth(ds->vec, i);
        if(haveUnion(curr,s)){
            SetUnion(curr,s);
            VectorDelete(ds->vec,i);
            VectorAppend(ds->vec,s);
        }
    }
    VectorAppend(ds->vec ,s);
}
VectorMap vecMap(void * elem , void * data){
    auxData * auxD = (auxData*)data;
    hashset  *currSet = (hashset*  ) elem;  
    auxD->wannaFind=HashSetLookup(currSet,auxD->wannaFind);
}

hashset * DisjointSetsFind(disjointsets *ds, void * elemAddr){
    auxData data;
    data.found=NULL;
    data.wannaFind = elemAddr;
    VectorMap(ds->vec,vecMap,&data);
    return data.found;
}

void DisjointSetsDispose(disjointsets *ds){
    for(int i=0; i<VectorLength(ds->vec); i++){
        HashSetDispose(&ds->vec[i]);
    }
    VectorDispose(ds->vec);
    free(ds);

}