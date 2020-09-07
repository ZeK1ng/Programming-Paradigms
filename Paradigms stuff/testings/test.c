#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

const int PHILO_NUM =5;
const int COXNA_AMOUNT= 10;
const int FORKS = 5;

typedef struct {
  int id;
  sem_t *lock;
}philo;

void think(int id){
  printf("%d th philo is thinking\n",id);
  sleep(rand()%2);
}
void philoEating(int id){
  printf("%dth philo is eating\n",id);
  sleep(rand()%2);
}
void * philoEat(void * philoStr){
  philo *currphilo = (philo*)philoStr;
  for(int i=0; i< COXNA_AMOUNT; i++){
    think(currphilo->id);
    if(currphilo->id >(currphilo->id+1)%PHILO_NUM){
      sem_wait(currphilo->lock+(currphilo->id+1)%PHILO_NUM);
      sem_wait(currphilo->lock+currphilo->id);
    }else{
      sem_wait(currphilo->lock+currphilo->id);
      sem_wait(currphilo->lock+(currphilo->id+1)%PHILO_NUM);
    }
    philoEating(currphilo->id);
    sem_post(currphilo->lock+currphilo->id);
    sem_post(currphilo->lock+(currphilo->id+1)%PHILO_NUM);
  }
  return NULL;
}

int main(){
  philo * phArr = malloc(PHILO_NUM * sizeof(philo));
  sem_t * locks = malloc(FORKS * sizeof(sem_t));

  for(int i=0; i< PHILO_NUM; i++){
    phArr[i].id=i;
    sem_init(locks+i,0,1);
    phArr[i].lock= locks;
  }  
  pthread_t philoTrheads[PHILO_NUM];
  for(int i=0; i< PHILO_NUM; i++){
    pthread_create(&philoTrheads[i],NULL,philoEat,&phArr[i]);
  }

  for(int i=0; i< PHILO_NUM; i++){
    pthread_join(philoTrheads[i],NULL);
  }
  
}