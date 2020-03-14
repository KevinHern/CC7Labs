#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

typedef struct {
	int * threadID;
	int * number;
} Arguments;

void *labThread(void * args)
{
	Arguments * arg = args; 
	int n = *(arg->number);
	int id = *(arg->threadID);

	int primes[n];
	int countPrimes = 1;
	* (primes) = 2;
	if (n)
	{
		printf("Thread %d: El numero %d es Primo.\n", *arg->threadID, 2);
	}
	int flag = 1;

	for (int i = 2; countPrimes < n; ++i)
	{
		if (i%2)
		{
			flag = 1;
			for (int j = 0; j < countPrimes; ++j)
			{
				if (i%primes[j])
				{
					continue;
				}
				else
				{
					flag = 0;
					break;
				}
			}

			if (flag)
			{
				*(primes + countPrimes) = i;
				++countPrimes;
				flag = 0;
				printf("Thread %d: El numero %d es Primo.\n", *arg->threadID, i);
			}
		}
		else
		{
			continue;
		}
	}
	return NULL; 
} 

// C Concepts 
// https://gist.github.com/Integralist/6c35d4f26c0c7bf5bdce7b95cfa906b3
int main ( int argc , char * argv []) {
	int * tmp = (int*)malloc(sizeof(int));
	printf("Ingrese Numero de Primos: ");
	scanf("%d", tmp);

	printf("\n"); 
	
	// Multithreading
	// https://www.geeksforgeeks.org/multithreading-c-2/
	pthread_t thread1, thread2;
	 
	//Arguments for each thread
	Arguments * args1 = malloc(sizeof(*args1));
	Arguments * args2 = malloc(sizeof(*args2));

	//Preparing arguments for thread 1
	args1->threadID = (int *)&thread1;
	args1->number = tmp;

	//Preparing arguments for thread 2
	args2->threadID = (int *)&thread2;
	args2->number = tmp;

	//Creating threads
	pthread_create(&thread1, NULL, labThread, args1); 
	pthread_create(&thread2, NULL, labThread, args2); 
	pthread_exit(NULL); 

	//Freeing Memory
	free(args1);
	free(args2);
	free(tmp);

	return 0;
}