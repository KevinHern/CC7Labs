#include <pthread.h>
#include <stdio.h>
#define NUMTHREADS 4
pthread_mutex_t gMutex; // ¿Por qué debe ser global?
int g_sum = 0;

void *threadFunc(void *arg) {
	pthread_mutex_lock( &gMutex );

	// los hilos acceden uno a la vez
	g_sum ++;

	printf ("Un thread cambio gsum %d ",g_sum);
	pthread_mutex_unlock( &gMutex );
	return NULL;
}

int main() {
	pthread_t hThread[NUMTHREADS];
	pthread_mutex_init( &gMutex, NULL );
	int i;
	for (i = 0; i < NUMTHREADS; i++)
		pthread_create(&hThread[i],NULL,threadFunc,NULL);

	for (i = 0; i < NUMTHREADS; i++) {
		pthread_join(hThread[i],NULL);
		printf("Thread %d finalizo \n",i);
	}
	printf ("Global sum = %d\n", g_sum);

	return 0;
}
