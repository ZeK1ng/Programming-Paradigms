#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct {
    int id;
    int * ith;
    sem_t * lock;
    int * ar;
    int len;
    int thrdCount;
}data;
/*  გვაქვს ნ სიგრძის მასივი და i ური ელემენტი უნდა დაბეჭდოს I%M ინდექსის სრედმა;
*/


void * func (void * args){
    data * currData = (data *) args;
    while(1){

        //sleep(1);
        sem_t* currSem = currData->lock+(currData->id);
        sem_wait(currSem);
        if(*(currData->ith)>=currData->len){
            sem_t *nextSem = currData->lock+(currData->id+1)%(currData->thrdCount);
            sem_post(nextSem);
            break;
        }
        printf("i: %d\n", *currData->ith);
        printf("thrId:%d printed %d\n",currData->id, currData->ar[*(currData->ith)]);
        (*currData->ith)++;
        sem_t *nextSem = currData->lock+(currData->id+1)%(currData->thrdCount);
        sem_post(nextSem);
    }
    return NULL;
}

int main(){
    int n,m;
    scanf("%d%d",&n,&m);
    int  * arr = malloc(sizeof(int) * n);
    for(int i=0; i< n; i++){
        arr[i]=i;
    } 
    pthread_t thrds[m];
    data * allData = malloc(sizeof(data)* m);
    sem_t * locks  = malloc(m * sizeof(sem_t));
    int ith = 0;

    // sem_init(locks,0,1);
    // for(int i = 1; i < m; i++){
    //     sem_init(locks+i,0,0);
    // }

    for(int i =0 ;i< m;i++){
        allData[i].id = i;
        allData[i].ar=arr;
        allData[i].ith = &ith;
        allData[i].len = n;
        allData[i].thrdCount = m;
        if(i==0){
         sem_init(locks+i,0,1);

        }else
        sem_init(locks+i,0,0);
        
        allData[i].lock = locks;   
    }

    for(int i=0; i< m; i++){
        pthread_create(thrds+i,NULL,func,allData+i);
    }
    
    for (int i=0; i< m; i++){
        pthread_join(thrds[i],NULL);
    }
    free(allData);
    for(int i=0; i< m; i++){
        sem_destroy(locks+i);  
    }

    return 0;
}