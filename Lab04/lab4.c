#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "Schedulers.h"
#include <string.h>

struct list requirements_queue;
struct disk_header * dheader;
struct node ** requirements;

/* READ REQUIREMENTS SECTION */

int read_requirement(){
	int tmp;
	printf("Disk Requirement: ");
	scanf("%d", &tmp);
	return tmp;
}

void read_requirements(){
	printf("### BEGIN ###\n");
	while (1){
		int op = read_requirement();
		if ( op > 100 ){
			printf("### END ###\n");
			break;
		}
		else {
			struct node * temp_requirement = malloc(sizeof(struct node));
			temp_requirement->block = op;
			temp_requirement->next = NULL;
			temp_requirement->prev = NULL;
			//printf("%d\n", op);
			add_back(&requirements_queue, temp_requirement);
		}
	}
	printf("%d\n", length(&requirements_queue));
}

/* MEMORY MANAGEMENT SECTION */

void malloc_all(){
	requirements = malloc(length(&requirements_queue)*sizeof(struct node));
	dheader = malloc(sizeof(struct disk_header));
}

void free_all() {
	free(requirements);
	free(dheader);
	free_list(&requirements_queue);
}

/* OTHER FUNCTIONS */
void init_dheader(){
	printf("Starting Disk Header Position: ");
	int pos;
	scanf("%d", &pos);
	printf("Starting Disk Header Direction (0 = left, 1 = right): ");
	int dir;
	scanf("%d", &dir);
	init_disk_header(dheader, pos, dir);
}


/* MAIN FUNCTION */

int main ( int argc , char * argv []) {
	printf("\n\n/*\tLaboratorio 4\t*/\n\n");
	/* Initialization of lists */
	init_list(&requirements_queue);

	// Reading every requirement
	printf("### DISK REQUIREMENTS ###\n");
	printf("Register the disk requirements one by one!!!!\n");
	read_requirements();

	//print_list(&requirements_queue);
	// Malloc everything usefule
	malloc_all();

	/* Initializing header */
	init_dheader();
	//print_list(&requirements_queue);
	

	/* Schedules */
	printf("### DISK SCHEDULERS ###\n\n");

	// FIFO SCHEDULE
	FIFO(dheader, &requirements_queue);

	// SCAN SCHEDULE
	order(&requirements_queue, 1);
	SCAN(dheader, &requirements_queue);



	//SSTF
	SSTF(dheader, &requirements_queue);


	//FREEING EVERY MALLOC 
	free_all();
	printf("List of Requirements must be empty now\n");
	return 0;
}