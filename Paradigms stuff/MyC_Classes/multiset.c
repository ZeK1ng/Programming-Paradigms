#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
typedef struct 
{
	void * elem;
	int amount; 
}data;
typedef struct 
{
	data * base;
	int allocLen;
	int logLen;
	size_t elemSize;
	int (* compareFn)(const void  * elem1, const void * elem2);
	void(*freeFn)(void * elem);
}multiSet;

//return pointer to a DataStruct that contains that element;
void* MlSetcontains(multiSet * mlset , void * elem){
	for(int i=0; i< mlset->logLen; i++){
		if(mlset->compareFn(mlset->base[i].elem, elem ) == 0){
			return &mlset->base[i];
		}
	}
	return NULL;
}

void initMultiSet( multiSet* mlset, size_t elemSize, int(* compareFn)(const void  * elem1, const void * elem2), void(*freeFn)(void * elem)){
	mlset->allocLen = 1;
	mlset->logLen = 0;
	mlset->elemSize = elemSize;
	mlset->base = malloc(mlset->allocLen * sizeof(data));
	mlset->compareFn= compareFn;
	mlset->freeFn = freeFn;
}
void grow(multiSet * mlset){
	mlset->allocLen*=2;
	//data * newBase =realloc(mlset->base,mlset->allocLen*sizeof(data)); 
	mlset->base =/*newBase; */realloc(mlset->base,mlset->allocLen*sizeof(data));
}

void MlSetAddElem(multiSet * mlset , void * elemAddr){
	data * isData = (data * )MlSetcontains(mlset,elemAddr);
	if(isData != NULL){
		isData->amount++;
		return;
	}
	if(mlset->logLen == mlset->allocLen){
		grow(mlset);
	}
	if(mlset->logLen ==0){
		data newData;
		newData.amount =1;
		newData.elem = malloc(mlset->elemSize);
		memcpy(newData.elem,elemAddr,mlset->elemSize);
		memcpy(mlset->base,&newData,sizeof(data)); 
		mlset->logLen++;
		return;
	}
	for(int i=0; i< mlset->logLen ; i++){		
		int k = mlset->compareFn(mlset->base[i].elem,elemAddr);
		if(k >0){

		//printf("baseElem%d  gadmElem%d\n",*(int *)(mlset->base[i].elem),*(int *)elemAddr );
			memmove((mlset->base )+ i+1,mlset->base + i, (mlset->logLen-i)*sizeof(data));
			data newData;
			newData.amount =1;
			newData.elem = malloc(mlset->elemSize);
			memcpy(newData.elem,elemAddr,mlset->elemSize);
			memcpy(mlset->base+i,&newData,sizeof(data)); 
			mlset->logLen++;

			return;
			//printf("amm_>%d, elem--%d",mlset->base[1].amount,*(int*)(mlset->base[1].elem));
		}
	}
	data newData;
	newData.amount =1;
	newData.elem = malloc(mlset->elemSize);
	memcpy(newData.elem,elemAddr,mlset->elemSize);
	memcpy(mlset->base+mlset->logLen,&newData,sizeof(data)); 

	mlset->logLen++;
}
int MlSetsize(multiSet * set){
	return set->logLen;
}
void deleteElem(multiSet* mlset, void * elem){
	assert(mlset->logLen>0);
	for (int i = 0; i < mlset->logLen; i++){
		if(mlset->compareFn(mlset->base[i].elem,elem) == 0){
			if(mlset->base[i].amount > 1){
				mlset->base[i].amount--;
			}else{
				if(mlset->freeFn != NULL)
				mlset->freeFn(mlset->base[i].elem);
				memmove(mlset->base + i, mlset->base+i+1,(mlset->logLen-i-1)*sizeof(data));
				mlset->logLen--;
				return;
			}
		}	
	}
}

int getCount(multiSet * mlset, void * elem){
	for(int i =0;i<mlset->logLen; i++){
		if(mlset->compareFn(mlset->base[i].elem,elem) == 0){
			return mlset->base[i].amount;
		}
	}
	return 0;
}

void * getNth(multiSet* mlset, int index){
	assert(index<mlset->logLen && index>=0);
	return(mlset->base[index].elem);
}
void destroy(multiSet * set){
	if(set->freeFn != NULL){
		for(int i=0; i<set->logLen; i++){
			set->freeFn(set->base[i].elem);
		}
	}
	free(set->base);
}

int  intCmpfn(const void * e1,const  void* e2){
	int n1 = *(int *)e1;
	int n2 = *(int *)e2;
	if(n1 == n2) return 0;
	if(n1>n2) return 1;
	return -1;
}
int StrCmp(const void* a,const  void* b) {
  return strcmp(*(char**)a, *(char**)b);
}

void StrFree(void* ptr) {
  free(*(char**)ptr);
}
void testInts(){
	printf("----testing ints-----\n");
		multiSet set;
	initMultiSet(&set,sizeof(int),intCmpfn,NULL);

	int k =4;
	int j =3;
	int h = 2;
	int g = 1;
	int m =0;
	int a = 10;
	MlSetAddElem(&set,&k);
	MlSetAddElem(&set,&k);
	MlSetAddElem(&set,&k);

	MlSetAddElem(&set,&j);
	MlSetAddElem(&set,&j);
	MlSetAddElem(&set,&j);
	MlSetAddElem(&set,&j);
	MlSetAddElem(&set,&j);
	MlSetAddElem(&set,&j);
	MlSetAddElem(&set,&j);

	MlSetAddElem(&set,&h);
	MlSetAddElem(&set,&h);
	MlSetAddElem(&set,&h);

	MlSetAddElem(&set,&g);
	MlSetAddElem(&set,&g);

	MlSetAddElem(&set,&m);
	MlSetAddElem(&set,&m);

	MlSetAddElem(&set,&a);

	printf("GetNth --%d\n",*(int * )getNth(&set,5) );
	printf("getCount -- %d\n", getCount(&set,&j) );


	//printf("getCount -- %d\n", getCount(&set,&j) );

	 printf("AllocLen is %d\n",set.allocLen );
	//MlSetAddElem(&set,&g);
	 printf("Size is %d\n",MlSetsize(&set) );
	// printf("AllocLen is %d\n",set.allocLen );
	for(int i=0; i< set.logLen; i++){
		printf("elem--%d,amm_>%d\n",*(int*)(set.base[i].elem),set.base[i].amount);

	}
	printf("---Deleting elements\n");
	deleteElem(&set,&h);
	deleteElem(&set,&h);
	deleteElem(&set,&h);
	deleteElem(&set,&a);
	for(int i=0; i< 7; i++){
		deleteElem(&set,&j);
	}
	for (int i = 0; i < 3; i++)
	{
		deleteElem(&set,&g);
	}
	for (int i = 0; i < 2; ++i)
	{
		deleteElem(&set,&m);
		
	}
	for (int i = 0; i < 3; ++i)
	{
		deleteElem(&set,&k);
	}
	printf("Size is %d\n",MlSetsize(&set) );
	printf("getCount -- %d\n", getCount(&set,&j) );
}
void testingStrings(){
	printf("------testing Strings -------\n");
	multiSet set;
	initMultiSet(&set,sizeof(char*),StrCmp,StrFree);
  	char *a = strdup("abc"), *b = strdup("xyz");
  	MlSetAddElem(&set,&b);
  	MlSetAddElem(&set,&b);
  	MlSetAddElem(&set,&b);
  	MlSetAddElem(&set,&b);

  	MlSetAddElem(&set,&a);
  	MlSetAddElem(&set,&a);
  	MlSetAddElem(&set,&a);
  	MlSetAddElem(&set,&a);
  	 printf("AllocLen is %d\n",set.allocLen );
	//MlSetAddElem(&set,&g);
	 printf("Size is %d\n",MlSetsize(&set) );
	 assert(0 == strcmp("abc", *(char**)getNth(&set, 0)));
	 for(int i=0; i< set.logLen; i++){
		printf("elem--%s,amm_>%d\n",*(char**)(set.base[i].elem),set.base[i].amount);

	}
	printf("-----deleting elements----\n");
	deleteElem(&set,&a);
	deleteElem(&set,&a);
	deleteElem(&set,&a);
	deleteElem(&set,&a);

	deleteElem(&set,&b);
	deleteElem(&set,&b);
	deleteElem(&set,&b);
	deleteElem(&set,&b);
	
		 for(int i=0; i< set.logLen; i++){
		printf("elem--%s,amm_>%d\n",*(char**)(set.base[i].elem),set.base[i].amount);

	}
	destroy(&set);

}

int main(){
	testInts();
	testingStrings();
	 return 0;
}


