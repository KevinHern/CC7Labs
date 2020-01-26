#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

void *myThread(void *vargp) {
	int *myid = (int *)vargp; 
	sleep(1);
	printf("TID: %d\n", *myid);
	return NULL; 
} 

// C Concepts 
// https://gist.github.com/Integralist/6c35d4f26c0c7bf5bdce7b95cfa906b3
int main ( int argc , char * argv []) {
	printf("### Begin Example ###\n"); 
	char* tmp = (char*)malloc(10 * sizeof(char));
	printf("Name: ");
	scanf("%s", tmp);
	printf("Thanks: %s\n", tmp);
	free(tmp);
	printf("\n"); 
	
	// Multithreading
	// https://www.geeksforgeeks.org/multithreading-c-2/
	pthread_t tid; 
	printf("v Before Thread\n"); 
	pthread_create(&tid, NULL, myThread, (void *)&tid ); 
	pthread_join(tid, NULL); 
	printf("^  After Thread\n"); 
	printf("\n"); 

	// New Process
	// https://www.geeksforgeeks.org/fork-system-call/
	pid_t pid ;
	printf("v Before Process\n"); 
	pid = fork ();
	pid = fork ();
	if ( pid == -1) {
		printf ("# Creation of a child process was unsuccessful.\n");
		return -1;
	} else {
		if( pid > 0){
			// Returned to parent or caller. The value contains process ID of newly created child process.
			printf ("PID %d Process Dad\n" , getpid () ) ;
		} else {
			// Returned to the newly created child process
			printf ("PID %d\tProcess Son\n" , getpid () );
		}
	}
	printf("^  After Process\n"); 
	return 0;
}