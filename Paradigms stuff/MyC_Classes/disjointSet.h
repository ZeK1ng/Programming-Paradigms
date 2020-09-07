#ifndef DSJS_SETS
#define DSJS_SETS
#include <stdio.h>
#include "vector.h"
#include "hashset.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    void * found;
    void * wannaFind;
}auxData;

typedef struct{
    bool yes;
}

typedef struct {
    vector * vec;
} disjointsets;

typedef struct {
    hashset * set;
} element;

void DisjointSetsNew(disjointsets *ds); 
void DisjointSetsAdd(disjointsets *ds, hashset *s); 
hashset * DisjointSetsFind(disjointsets *ds, void * elemAddr); 
void DisjointSetsDispose(disjointsets *ds); 
#endif