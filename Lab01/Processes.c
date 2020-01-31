#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

void labProc(int id, int number)
{
	int procId = id;
	int n = number;

	int primes[n];
	int countPrimes = 1;
	* (primes) = 2;
	if(n){
		printf("Process %d: El numero %d es Primo.\n", procId, 2);
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
				printf("Process %d: El numero %d es Primo.\n", procId, i);
			}
		}
		else
		{
			continue;
		}
	}
} 

// C Concepts 
// https://gist.github.com/Integralist/6c35d4f26c0c7bf5bdce7b95cfa906b3
int main ( int argc , char * argv []) {
	int * tmp = (int*)malloc(sizeof(int));
	printf("Ingrese Numero de Primos: ");
	scanf("%d", tmp);

	printf("\n"); 

	pid_t pid ;

	//Creating new Process
	pid = fork ();
	
	//Both Processes will execute this function
	labProc(getpid(), *tmp);

	free(tmp);

	return 0;
}