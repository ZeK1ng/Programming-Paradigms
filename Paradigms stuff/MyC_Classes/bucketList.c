#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef  int(*CompFn)( void * v1,  void * v2);
typedef void (*  FreeFn)(void * v1);

typedef struct
{
	void * elem;
	size_t elemSize;
	FreeFn freefn;
	int amount;
}data;

typedef struct 
{
	data * base;
	CompFn cmpfn;
	int loglen;
	int alloclen;
}buckets;

void bucketsListNew(buckets * bcks,CompFn cmp){
	bcks->loglen =0;
	bcks->alloclen = 10;
	bcks->base = malloc(bcks->alloclen * sizeof(data));
	bcks->cmpfn = cmp;
}
void bucketsListDispose(buckets * bcks){
	for(int i=0; i<bcks->loglen ; i++){
		if(bcks->base[i].freefn != NULL) {
			bcks->base[i].freefn(bcks->base[i].elem);
		}
		free(bcks->base[i].elem);

	}
	free(bcks->base);
}
void checkAndGrow(buckets * bcks){
	if(bcks->loglen == bcks ->alloclen){
		bcks->alloclen *=2;
		bcks->base= realloc(bcks->base,bcks->alloclen * sizeof(data));
	}
}
data * bckListContains(buckets * bcks,void * elem){
	for(int i =0; i< bcks->loglen ; i++){
		if(bcks->cmpfn(bcks->base[i].elem,elem) == 0){
			return &bcks->base[i];
		}
	}
	return NULL;
}
int getSize(buckets * bcks){
	return bcks->loglen;
}

void * getAtIndex(buckets * bcks,int i){
	return bcks->base[i].elem;
}
void bucketsListAdd(buckets *bcks, void * elem,size_t elemSize, FreeFn freefn){
	checkAndGrow(bcks);
	data * isData = bckListContains(bcks,elem);
	if(isData != NULL){
		isData->amount++;
		return;
	}
	data newData;
	newData.amount =1;
	newData.elem = malloc(elemSize);
	newData.elemSize=elemSize;
	newData.freefn=freefn;
	memcpy(newData.elem,elem,elemSize);
	bcks->loglen++;

	bcks->base[bcks->loglen-1]=newData;

}
int getAmount(buckets * bcks, void* elem){
	data * isdata = bckListContains(bcks,elem);
	if(isdata != NULL) return isdata->amount;
	return 0;
}
void * getMax(buckets * bcks){
return NULL;
}
void swapElem(buckets * bcks,int first, int sec){
	if(sec < first){
		int temp = first;
		first = sec;
		sec= temp;
	}
	data * newbase = malloc(sizeof(data) * bcks->loglen);
	memcpy(newbase,bcks->base,first*sizeof(data));
	memcpy(newbase+first,bcks->base+sec,sizeof(data));
	memcpy(newbase+first+1,bcks->base+first+1,sizeof(data)*(sec-first-1));
	memcpy(newbase+sec,bcks->base+first,sizeof(data));
	memcpy(newbase+sec+1,bcks->base+sec+1,sizeof(data)*(bcks->loglen-sec));
	free(bcks->base);
	bcks->base = newbase;
}
void sortBucks(buckets * bcks){
	int minIndx,i,j;
	for( i=0; i<bcks->loglen-1 ; i++){
		minIndx = i;
		for(j= i+1; j<bcks->loglen; j++){
			if(bcks->cmpfn(bcks->base[j].elem,bcks->base[minIndx].elem)<0){
				minIndx = j;
			}
		}
		swapElem(bcks,i,j)	;
	}0

}


void deleteIndex(buckets * bcks, int index){
	if(bcks->base[index].amount>1){
		bcks->base[index].amount--;
		return;
	}
	if(bcks->base[index].freefn != NULL){
		bcks->base[index].freefn(bcks->base[index].elem);
	}
	free(bcks->base[index].elem);
	memmove(bcks->base+index,bcks->base+index+1,sizeof(data)*(bcks->loglen-index-1));
	bcks->loglen--;
}

int cmpfn(void * e1,void * e2){
	int n1 = *(int *)e1;
	int n2 = *(int *)e2;
	if(n1 == n2) return 0;
	if(n1>n2) return 1;
	return -1;
}

void testints(){
	buckets bcks;
	
	bucketsListNew(&bcks,cmpfn);
	printf("----adding elements -------\n");
	// for (int i = 0; i < 10; i++)
	// {
	// 	bucketsListAdd(&bcks,&i,sizeof(int),NULL);
	// }
	// for (int i = 0; i < 10; ++i)
	// {
	// 	bucketsListAdd(&bcks,&i,sizeof(int),NULL);
	// }
	int i =2;
 	bucketsListAdd(&bcks,&i,sizeof(int),NULL);
 	i =4;
 	bucketsListAdd(&bcks,&i,sizeof(int),NULL);
 	i=1;
 	bucketsListAdd(&bcks,&i,sizeof(int),NULL);
 	i=10;
 	bucketsListAdd(&bcks,&i,sizeof(int),NULL);
 	i = 18;
 	bucketsListAdd(&bcks,&i,sizeof(int),NULL);



	printf("-----gettting elements and their amounts\n");
	for (int i = 0; i < bcks.loglen; i++)
	{
		printf("%d ---> am: %d\n",*(int*)getAtIndex(&bcks,i),getAmount(&bcks,getAtIndex(&bcks,i)));
		
	}
	//swapElem(&bcks,0, bcks.loglen-1);
	sortBucks(&bcks);
	printf("-----gettting elements and their amounts\n");
	for (int i = 0; i < bcks.loglen; i++)
	{
		printf("%d ---> am: %d\n",*(int*)getAtIndex(&bcks,i),getAmount(&bcks,getAtIndex(&bcks,i)));
		
	}
	// printf("---deleting elements----\n" );
	// deleteIndex(&bcks,3);
	// deleteIndex(&bcks,3);
	// deleteIndex(&bcks,0);
	// deleteIndex(&bcks,0);

	
	// printf("-----gettting elements and their amounts\n");
	// for (int i = 0; i < bcks.loglen; i++)
	// {
	// 	printf("%d ---> am: %d\n",*(int*)getAtIndex(&bcks,i),getAmount(&bcks,getAtIndex(&bcks,i)));
		
	// }
	bucketsListDispose(&bcks); 



}


int main(){
	testints();

	return 0;
}