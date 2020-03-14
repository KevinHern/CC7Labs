#include "DiskHeader.h"
#include "list.h"
#include <math.h>

double std(float * distances, float mean, int length) {
	double acc = 0;
	for (int i = 0; i < length; ++i)
	{
		//printf("%d.\tDistance: %0.2f\tMean: %0.2f\t", i, distances[i], mean);
		double tmp = pow(distances[i] - mean, 2);
		//printf("Diff: %0.2f\tPow: %0.2f\n", distances[i] - mean, (float)tmp);
		acc += pow(distances[i] - mean, 2);
	}
	//printf("Acc: %0.2f\t", (float)acc);
	acc = (float)(acc/(length-1));
	//printf("Division: %0.2f\t", acc);

	//printf("SQRT: %0.2f\n", sqrt(acc));
	return sqrt(acc);
}

void FIFO(struct disk_header * dheader, struct list * l) {
	float * distances = malloc(length(l) * sizeof(int));
	int count = 0;
	struct node * tmp = &l->head;
	float average = 0;

	printf("\n\nFirs In First Out\n");
	printf("Starting Header Position: %d\n", dheader->initialBlock);
	
	printf("Dispatched List: ");
	
	while((tmp = tmp->next) != NULL){
		printf("%d, ", tmp->block);
		int distance = abs(dheader->currentBlock - tmp->block);
		*(distances + count) = distance;
		average += distance;

		dheader->currentBlock = tmp->block;
		count++;
	}

	printf("IDLE\n");

	average = (float)(average / length(l));
	printf("Average: %0.2f\n", average);


	double sd = std(distances, average, length(l));
	printf("Standard Deviation: %0.2f\n", sd);

	reset_disk_header(dheader);
	free(distances);
	
}

void SCAN(struct disk_header * dheader, struct list * l) {
	float * distances = malloc(length(l) * sizeof(int));
	int count = 0;
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
			*(distances + count) = distance;
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
			count++;
		}

		/* Overhead caused by the reason the header needs to do a full scan from side to side */

		int overhead = abs(100 - dheader->currentBlock);
		average += overhead;
		dheader->currentBlock = 100;

		for (pivot = prev(start); pivot != NULL; pivot = prev(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			*(distances + count) = overhead + distance;
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
			count++;
			overhead = 0;
		}

	}
	else{
		if (start->block > dheader->initialBlock)
		{
			start = prev(start);
		}
		else
		{
			while(start->block == next(start)->block){
				start = next(start);
			}
		}


		for (pivot = start; pivot != NULL; pivot = prev(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			*(distances + count) = distance;
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
			count++;
		}

		/* Overhead caused by the reason the header needs to do a full scan from side to side */

		int overhead = abs(1 - dheader->currentBlock);
		average += overhead;
		dheader->currentBlock = 1;

		for (pivot = next(start); pivot != NULL; pivot = next(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			*(distances + count) = overhead + distance;
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
			count++;
			overhead = 0;
		}
	}

	printf("IDLE\n");

	average = (float)(average / length(l));
	printf("Average: %0.2f\n", average);

	double sd = std(distances, average, length(l));
	printf("Standard Deviation: %0.2f\n", sd);

	reset_disk_header(dheader);
	free(distances);
}

void CSCAN(struct disk_header * dheader, struct list * l) {
	float * distances = malloc(length(l) * sizeof(int));
	int count = 0;
	float average = 0;

	printf("\n\nCIRCULAR SCAN\n");
	printf("Starting Header Position: %d\n", dheader->initialBlock);
	
	

	struct node * start = starting_block(l, dheader->initialBlock);

	struct node * pivot;

	printf("Dispatched List: ");

	if (dheader->initialDirection) {
		for (pivot = start; pivot != NULL; pivot = next(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			*(distances + count) = distance;
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
			count++;
		}

		/* Overhead caused by the reason the header needs to finish the scan run and returns to the other side immediately */

		int overhead = abs(100 - dheader->currentBlock) + 100 ;
		average += overhead;
		dheader->currentBlock = 1;

		for (pivot = l->head.next; pivot != start; pivot = next(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			*(distances + count) = overhead + distance;
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
			count++;
			overhead = 0;
		}

	}
	else{
		if (start->block > dheader->initialBlock)
		{
			start = prev(start);
		}
		else
		{
			while(start->block == next(start)->block){
				start = next(start);
			}
		}


		for (pivot = start; pivot != NULL; pivot = prev(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			*(distances + count) = distance;
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
			count++;
		}

		/* Overhead caused by the reason the header needs to finish the scan run and returns to the other side immediately */

		int overhead = abs(1 - dheader->currentBlock) + 100 ;
		average += overhead;
		dheader->currentBlock = 100;

		for (pivot = l->tail.prev; pivot != start; pivot = prev(pivot))
		{
			printf("%d, ", pivot->block);
			int distance = abs(dheader->currentBlock - pivot->block);
			*(distances + count) = overhead + distance;
			average += distance;
			//printf("distance: %d\n", distance);
			dheader->currentBlock = pivot->block;
			count++;
			overhead = 0;
		}

	}

	printf("IDLE\n");

	average = (float)(average / length(l));
	printf("Average: %0.2f\n", average);

	double sd = std(distances, average, length(l));
	printf("Standard Deviation: %0.2f\n", sd);

	reset_disk_header(dheader);
	free(distances);
}

void SSTF(struct disk_header * dheader, struct list * l) {
	float * distances = malloc(length(l) * sizeof(int));
	int count = 0;
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
		*(distances + count) = distance;
		average += distance;
		//printf("distance: %d\n", distance);
		dheader->currentBlock = tmp->block;
		count++;
		free(tmp);
	}

	printf("IDLE\n");

	average = (float)(average / length(l));
	printf("Average: %0.2f\n", average);

	double sd = std(distances, average, length(l));
	printf("Standard Deviation: %0.2f\n", sd);
	
	reset_disk_header(dheader);
	free(distances);
}