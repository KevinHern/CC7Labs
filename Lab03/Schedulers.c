#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include "List.h"
#include "fcfs.h"
#include <string.h>

struct list queue;

void replace_char(char * str, char find, char replace){
	int i = 0;
	while(*(str+i)!='\0')
    {
        if(*(str+i) == find)
        {
            *(str+i) = replace;
        }
        i++;
    }
}

void create_process(char * str){
	char * copy = malloc(50*sizeof(char));

	int j = 0;
	for (; str[j] != '\0'; ++j)
	{
		copy[j] = str[j];
	} 
	copy[j] = '\0';

	
	char *ptr = strtok(copy, "\t");

	struct node * process = malloc(sizeof(struct list));
	process->prev = NULL;
	process->next = NULL;

	process->name = malloc(5*sizeof(char));
	
	int i = 0;
	for (; ptr[i] != '\0'; ++i)
	{
		process->name[i] = ptr[i];
	} 
	process->name[i] = '\0';

	
	ptr = strtok(NULL, "\t");
	process->at = atoi(ptr);
	

	ptr = strtok(NULL, "\t");
	process->bt = atoi(ptr);


	ptr = strtok(NULL, "\t");
	process->v = atoi(ptr);
	

	add_back(&queue, process);
	free(copy);
	
}

void free_list(){

	struct node * n;

	while((n = pop(&queue)) != NULL ){
		free(n->name);
		free(n);
	}
}

void load_processes(){
	init_list(&queue);

	FILE * fp = fopen("listProcess.cpu", "r");
	
	char ch = fgetc(fp);
	for (int i = 0; i < 11; ++i)
	{
		char ch = fgetc(fp);
	}

	char * buffer = malloc(50*sizeof(char));
	uint i = 0;

	while((ch = fgetc(fp)) != EOF){
		if (ch == '\n')
		{
			*(buffer + i) = '\0';
			create_process(buffer);
			//printf("%s\n", buffer);
			*buffer = '\0';
			i = 0;
		}
		else
		{
			*(buffer + i) = ch;
			i++;
		}

		
	}
	free(buffer);

	//printf("%d\n", length(&queue));

	fclose(fp);
}

void output(void *vargp) {
	printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n");
	printf("|PXX\t|QU\t|QM\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n");
	printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n");

	struct node *n = &queue.head;
	while((n = n->next) != NULL){
		printf("|%s\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
			n->name, n->qu, n->qm, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
		printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n");
	}
} 

void FCFS(void *vargp) {
	
	




	printf(" --> FCFS\n");
	output(0);
} 

void PS(void *vargp) {
	printf(" --> PS\n");
	output(0);
} 

void RR(void *vargp) {
	printf(" --> RR\n");
	output(0);
} 

void MLFQS(void *vargp) {
	printf(" --> MLFQS\n");
	output(0);
} 

int menu(){
	int tmp;
	printf("############\n1 - FCFS\n2 - PS\n3 - RR\n4 - MLFQS\n0 - EXIT\n\nSeleccione Opción: ");
	scanf("%d", &tmp);
	return tmp;
}

// C Concepts 
// https://gist.github.com/Integralist/6c35d4f26c0c7bf5bdce7b95cfa906b3
int main ( int argc , char * argv []) {
	printf("\n\n\n/*\tTERMINAR EL PROGRAMA USANDO LA OPCION 0, HAY MALLOCS INVOLUCRADOS!!\t*/\n");
	load_processes();
	int op = 9;
	pid_t pid;
	while (op > 0){
		op = menu();
		printf("Procesando opción: %d\n", op);
		if ( op >5 || op<=0 ){
			printf("### END ###\n");
			break;
		}
		printf("### BEGIN ###\n");
		switch(op){
			case 1:
			FCFS(0);
			break;
			case 2:
			PS(0);
			break;
			case 3:
			RR(0);
			break;
			default:
			case 4:
			MLFQS(0);
			break;
		}
	}

	//FREEING EVERY MALLOC 
	free_list();
	printf("List of processes must be empty now: ");
	printf("%d", length(&queue)); //Number displayed should be 0!!!!!!!!!
	printf("\tprocesses\n");
	return 0;
}