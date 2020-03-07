#include <stdio.h>
#include <pthread.h>
void *hello (void * arg) { 
	printf("Hello Thread\n");
	return NULL;
}
int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, hello, NULL);
	return 0;
}
