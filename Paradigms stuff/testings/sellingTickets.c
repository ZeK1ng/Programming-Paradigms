#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct 
{
	int * allTicks;
	int id;
	sem_t * lock;
}data;

void *sellTickets (void * args){
	data * curr = (data * )args;
	int sold=0;
	while(1){
		sem_wait(curr->lock);

		//printf("%d\n",*(curr->allTicks) );
		if(*(curr->allTicks) == 0){
			sem_post(curr->lock);

			break;		
		} 
		(*(curr->allTicks))--;
		sold++;
		printf("id-> %d, sold so far -> %d\n",curr->id,sold);
		sem_post(curr->lock);
		usleep(20);
	}
	printf("All tickets are sold. i wiht id %d sold %d tickets\n ",curr->id,sold);
	return NULL;
}
int main(){

	data curr[10];

	pthread_t thds[10];
	int * sulTick = malloc(sizeof(int));
	*sulTick = 100;
	sem_t * lock = malloc(sizeof(sem_t));
	sem_init(lock,0,1);
	for(int i=0; i< 10; i++){
		curr[i].allTicks = sulTick;
		curr[i].id =i;
		curr[i].lock = lock;
		pthread_create(&thds[i],NULL,sellTickets,&curr[i]);
	}
	for(int i=0; i< 10; i++){
		pthread_join(thds[i],NULL);
	}
	free(sulTick);
	sem_destroy(lock);
	free(lock);
	return 0;
}