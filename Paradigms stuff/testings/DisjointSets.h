#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hashset.h"
#include "vector.h"

typedef struct{
    vector v_sets;
} disjointsets;

typedef struct{
    void * result;
    void * elemAddr;
} PP;

typedef struct{
    disjointsets * ds;
    hashset * hs;
    void * result;
} Pointers;

void DisjointSetsNew(disjointsets *ds); 
void DisjointSetsAdd(disjointsets *ds, hashset *s); 
hashset * DisjointSetsFind(disjointsets *ds, void * elemAddr); 
void DisjointSetsDispose(disjointsets *ds); 