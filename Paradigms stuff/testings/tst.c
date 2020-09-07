#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct 
{
	void * k;
	int m;

}str;

int main(){
	str * data = malloc(3* sizeof(str));
	int k = 4;

	str curr;
	curr.k= malloc(sizeof(int));
	curr.m = 11;

	memcpy(curr.k,&k,sizeof(int));

	memcpy(data,&curr,sizeof(str));

	printf("%d %d",*(int*)(data[0].k),((str*)data)->m);
}