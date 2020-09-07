#include "DisjointSets.h"

void InsertInSet(void * elemAddr, void * auxData)
{
    hashset * hs = (hashset*)auxData;
    HashSetEnter(hs, elemAddr);
}

void UnionSets(hashset * hs1, hashset * hs2)
{
    HashSetMap(hs2, InsertInSet, hs1);
}

void HashSetFree(void * elemAddr)
{
    HashSetDispose((hashset*)elemAddr);
}

void DisjointSetsNew(disjointsets *ds)
{
    VectorNew(&ds->v_sets, sizeof(hashset), HashSetFree, 0);
}

void UnionIfFound(void * elemAddr, void * auxData)
{
    Pointers * pointers = (Pointers*)auxData;
    hashset * found = DisjointSetsFind(pointers->ds, elemAddr);
    if(found != NULL)
    {
        UnionSets(pointers->hs, found);
        void * base = VectorNth(&pointers->ds->v_sets, 0);
        int index = ((int)found - (int)base) / sizeof(hashset);
        VectorDelete(&pointers->ds->v_sets, index);
    }
}

void FindFirstToUnion(void * elemAddr, void * auxData)
{
    Pointers * pointers = (Pointers*)auxData;
    if(pointers->result != NULL) return;
    hashset * found = DisjointSetsFind(pointers->ds, elemAddr);
    pointers->result = found;
}

hashset * GetFirstSetToUnion(disjointsets * ds, hashset * hs)
{
    Pointers pointers;
    pointers.ds = ds;
    pointers.hs = hs;
    pointers.result = NULL;
    HashSetMap(hs, FindFirstToUnion, &pointers);
    return pointers.result;
}

void DisjointSetsAdd(disjointsets *ds, hashset *hs)
{
    hashset * firstToUnion = GetFirstSetToUnion(ds, hs);
    if(firstToUnion != NULL)
    {
        UnionSets(firstToUnion, hs);
        Pointers pointers;
        pointers.ds = ds;
        pointers.hs = firstToUnion;
        HashSetMap(hs, UnionIfFound, &pointers);
        HashSetDispose(hs);
    }
    else    // nothing to union with
    {
        VectorAppend(&ds->v_sets, hs);
    }
}

void FindInVector(void * eachElemAddr, void * auxData)
{
    hashset * hs = (hashset*)eachElemAddr;
    PP * pp = (PP*)auxData;
    void * found = HashSetLookup(hs, pp->elemAddr);
    if(pp->result == NULL)
        pp->result = found;
}

hashset * DisjointSetsFind(disjointsets *ds, void * elemAddr)
{
    PP pp;
    pp.result = NULL;
    pp.elemAddr = elemAddr;
    VectorMap(&ds->v_sets, FindInVector, &pp);
    return pp.result;
}

void DisjointSetsDispose(disjointsets *ds)
{
    VectorDispose(&ds->v_sets);
}

/* ---------------------------------- Test ---------------------------------- */

int main()
{
    //disjointsets ds;
    //DisjointSetsNew(&ds);
    return 0;
}