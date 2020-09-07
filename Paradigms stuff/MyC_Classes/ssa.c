#include "ssa.h"
#include <stdlib.h>
#include "vector.h"
#include <stdio.h>
#include <strings.h>
#include <string.h>

static void freeString(void *str) {
   free(*(char **) str); 
}
void SSANew(sparsestringarray *ssa, int arrayLength, int groupSize){
  ssa->arrayLength = arrayLength;
  ssa->groupSize = groupSize;
  ssa->numGroups = (arrayLength -1)/ groupSize+1 ;
  ssa->groups = malloc(ssa->numGroups * sizeof(group));
  for(int i= 0; i< ssa->numGroups ; i++){
    ssa->groups[i].bitmap = malloc(ssa->groupSize * sizeof(bool));
    bzero(ssa->groups[i].bitmap,groupSize*sizeof(bool));
    VectorNew(&ssa->groups[i].strings,sizeof(char * ),freeString,0);
  }

}
void SSADispose(sparsestringarray *ssa){
  for(int i =0; i< ssa->numGroups; i++){
    free(ssa->groups[i].bitmap);
    VectorDispose(&ssa->groups[i].strings);
  }
  free(ssa->groups);
}
bool SSAInsert(sparsestringarray *ssa, int index, const char *str){
  int groupNum = index / ssa->groupSize;
  int bitMIndex = index % ssa->groupSize;
  int vectorInd = 0;
  for(int i = 0; i< bitMIndex; i++){
    if(ssa->groups[groupNum].bitmap[i]) vectorInd++;
  }
  char * cp = strdup(str);
  if(ssa->groups[groupNum].bitmap[bitMIndex]){
    VectorReplace(&ssa->groups[groupNum].strings,&str,vectorInd); 
  }else{
      VectorInsert(&ssa->groups[groupNum].strings,&str,vectorInd);
      ssa->groups->bitmap[bitMIndex] = true;
    }
  return !ssa->groups->bitmap[bitMIndex];
}

void SSAMap(sparsestringarray *ssa, SSAMapFunction mapfn, void *auxData){

}