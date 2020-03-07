#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 4
void *hello (void *arg) { 
	printf("Hello Thread\n"); 
	return NULL;
}

int main() {	
	pthread_t tid[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++)
		pthread_create(&tid[i], NULL, hello, NULL);
	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(tid[i], NULL);

	return 0;
}
