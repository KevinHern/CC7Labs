#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"
#include <string.h>

struct list queue;
struct list priority_queue;
struct list queue_rr1;
struct list queue_rr2;
struct node ** processes;

uint nprocess;

int set_quantum(){
	int qm;
	scanf("%d", &qm);

	for (int i = 0; i < nprocess; ++i)
	{
		(*(processes + i))->qm = qm;
	}
	return qm;
}

void reset_processes(){
	for (int i = 0; i < nprocess; ++i)
	{
		(*(processes + i))->workDone = 0;
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

void free_processes(){

	for (int i = 0; i < nprocess; ++i)
	{
		free((*(processes + i))->name);
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
	fclose(fp);
}


void FCFS(void *vargp) {
	printf(" --> FCFS\n");
	init_list(&queue);
	fcfs_fill_queue(&queue, processes, nprocess);
	fcfs_schedule(&queue);
	output(processes, nprocess, 0);
} 

void PS(void *vargp) {	
	printf(" --> PS\n");
	init_list(&priority_queue);
	ps_schedule(&priority_queue, processes, nprocess);
	output(processes, nprocess, 1);
} 

void RR(void *vargp) {
	init_list(&queue);
	printf(" --> RR\n");
	printf("Ingrese el 'Quantum' que desee para el procesador: ");
	int quantum =  set_quantum();
	reset_processes();
	rr_schedule(&queue, processes, nprocess, quantum);
	output(processes, nprocess, 2);
} 

void MLFQS(void *vargp) {
	init_list(&queue);
	init_list(&queue_rr1);
	init_list(&queue_rr2);
	printf(" --> MLFQS\n");
	printf("Ingrese el 'Quantum' que desee para la cola Round Robin 1: ");
	int quantum1 =  set_quantum();
	printf("Ingrese el 'Quantum' que desee para la cola Round Robin 2: ");
	int quantum2 =  set_quantum();
	reset_processes();
	mlfqs_schedule_no_priority(&queue_rr1, &queue_rr2, &queue, processes, nprocess, quantum1, quantum2);
	output(processes, nprocess, 3);
} 

void fill_process_array(){
	nprocess = length(&queue);
	processes = malloc(nprocess*sizeof(struct list));

	for (int i = 0; i < nprocess; ++i)
	{
		*(processes + i) = pop(&queue); 
	}
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

	printf("\n\n\n/*\tARCHIVO QUE TIENE LOS PROCESOS (listprocess.cpu) DEBE TERMINAR CON UNA NUEVA LINEA PARA QUE EL PARSEO DEL ARCHIVO FUNCIONE!!\t*/\n\n");
	printf("\n\n\n/*\tTERMINAR EL PROGRAMA USANDO LA OPCION 0, HAY MALLOCS INVOLUCRADOS!!\t*/\n");
	load_processes();
	fill_process_array();
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
	free_processes();
	printf("List of processes must be empty now\n");
	free(processes);
	return 0;
}