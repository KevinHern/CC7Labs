#include "DiskHeader.h"
#include "list.h"

void FIFO(struct disk_header * dheader, struct list * l) {
	struct node * tmp = &l->head;
	float average = 0;

	printf("\n\nFirs In First Out\n");
	printf("Starting Header Position: %d\n", dheader->initialBlock);
	
	printf("Dispatched List: ");
	
	while((tmp = tmp->next) != NULL){
		printf("%d, ", tmp->block);
		int distance = abs(dheader->currentBlock - tmp->block);
		average += distance;

		dheader->currentBlock = tmp->block;
	}

	printf("IDLE\n");

	average = average / length(l);
	printf("Average: %0.2f\n", average);

	printf("Standard Deviation: \n");

	reset_disk_header(dheader);
	
}

void SCAN(struct disk_header * dheader, struct list * l) {
	float average = 0;

	printf("\n\nSCAN\n");
	printf("Starting Header Position: %d\n", dheader->initialBlock);
	
	printf("Dispatched List: ");

	struct node * start = starting_block(l, dheader->initialBlock);
	struct node * pivot;

	if (dheader->initialDirection) {
		for (pivot = start; pivot != NULL; pivot = next(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
		}

		for (pivot = prev(start); pivot != NULL; pivot = prev(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
		}

	}
	else{
		struct node * temp = next(start);
		if (temp != NULL)
		{
			while(start->block == temp->block){
				start = temp;
				temp = next(start);
			}
		}


		for (pivot = start; pivot != NULL; pivot = prev(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
		}

		for (pivot = next(start); pivot != NULL; pivot = next(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
		}

	}

	printf("IDLE\n");

	average = average / length(l);
	printf("Average: %0.2f\n", average);

	printf("Standard Deviation: \n");

	reset_disk_header(dheader);
}

void CSCAN(struct disk_header * dheader, struct list * l) {
	

	reset_disk_header(dheader);
}

void SSTF(struct disk_header * dheader, struct list * l) {

	struct list temporal;
	init_list(&temporal);
	copy(l, &temporal);



	float average = 0;

	printf("\n\nShortest Seek Time First\n");
	printf("Starting Header Position: %d\n", dheader->initialBlock);
	
	//print_list(&temporal);

	printf("Dispatched List: ");

	

	while(length(&temporal) != 0){
		struct node * tmp = nearest_block(&temporal, dheader->currentBlock);
		remove_node(&temporal, tmp);
		printf("%d, ", tmp->block);
		int distance = abs(dheader->currentBlock - tmp->block);
		average += distance;

		dheader->currentBlock = tmp->block;
		free(tmp);
	}

	printf("IDLE\n");

	average = average / length(l);
	printf("Average: %0.2f\n", average);

	printf("Standard Deviation: \n");
	
	reset_disk_header(dheader);
}