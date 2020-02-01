#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
	pid_t pid;
	pid = fork(); /* fork a child process */
	if (pid < 0) {			/* error occurred */
		fprintf(stderr, "Fork Failed\n");
		exit(-1);
	} else if (pid == 0) {	/* child process */
		printf("I am the child %d\n",pid);
		execlp("/bin/ls","ls",NULL);
	} else {				/* parent process */
		/* parent will wait for the child to complete*/
		printf("I am your father %d\n",pid);
		wait(NULL);
		printf("Child Complete: I’ll never join you!! \n");
		exit(0);
	}
}