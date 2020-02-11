#include <stdlib.h>
#include "List.h"

// variable 'mode' is used to determine what to display on screen
// Mode = 1 is Priority Scheduling
// Mode = 2 is Round Robin
// Mode = 0 Every other schedule

void throughput(struct node ** array, uint length, int mode){
	float pr, qu, qm, at, bt, et, wt, rt, tt;
	pr = 0;
	qu = 0;
	qm = 0;
	at = 0;
	bt = 0;
	et = 0;
	wt = 0;
	rt = 0;
	tt = 0;

	for (int i = 0; i < length; ++i)
	{
		pr = pr + (*(array + i))->v;
		qu = qu + (*(array + i))->qu;
		qm = qm + (*(array + i))->qm;
		at = at + (*(array + i))->at;
		bt = bt + (*(array + i))->bt;
		et = et + (*(array + i))->et;
		wt = wt + (*(array + i))->wt;
		rt = rt + (*(array + i))->rt;
		tt = tt + (*(array + i))->tt;
	}

	pr = (float)(pr / length);
	qu = (float)(qu / length);
	qm = (float)(qm / length);
	at = (float)(at / length);
	bt = (float)(bt / length);
	et = (float)(et / length);
	wt = (float)(wt / length);
	rt = (float)(rt / length);
	tt = (float)(tt / length);

	if (mode == 1)
	{
		printf("|TH\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t| \n",
			 pr, at, bt, et, wt, rt, tt);
	}
	else if(mode == 2)
	{
		printf("|TH\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t| \n",
			 qu, qm, at, bt, et, wt, rt, tt);
	}
	else
	{
		printf("|TH\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t| \n",
			 at, bt, et, wt, rt, tt);
	}
}

void output(struct node ** array, uint length, int mode) {
	char * separator;
	char * header;

	if (mode == 1)
	{
		separator = "|-------|-------|-------|-------|-------|-------|-------|-------| \n";
		header = "|PXX\t|PR\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n";
	}
	else if (mode == 2)
	{
		separator = "|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n";
		header = "|PXX\t|QU\t|QM\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n";
	}
	else
	{
		separator = "|-------|-------|-------|-------|-------|-------|-------| \n";
		header = "|PXX\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n" ;
	}

	printf("%s", separator);
	printf("%s", header);
	printf("%s", separator);

	struct node * n;

	if (mode == 1)
	{
		for (int i = 0; i < length; ++i)
		{
			n = *(array + i);
			printf("|%s\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				n->name, n->v, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
			printf("%s", separator);
		}
	}
	else if (mode == 2)
	{
		for (int i = 0; i < length; ++i)
		{
			n = *(array + i);
			printf("|%s\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				n->name, n->qu, n->qm, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
			printf("%s", separator);
		}
	}
	else
	{
		for (int i = 0; i < length; ++i)
		{
			n = *(array + i);
			printf("|%s\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				n->name, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
			printf("%s", separator);
		}
	}

	throughput(array, length, mode);
	printf("%s", separator);
} 

void fcfs_fill_queue(struct list *queue, struct node ** array, int length){
	for (int i = 0; i < length; ++i)
	{
		add_back(queue, *(array + i));
	}
}

void fcfs_schedule(struct list *l){
	int at = 0;
	struct node * original = &l->head;
	struct node * n;
	uint kernel_time = 0;

	int numberProcesses = length(l);

	int dispatched = 0;

	for (int at = 0; dispatched != numberProcesses; ++at)
	{
		n = original;
		while((n = n->next) != NULL){
			if (n->at == at)
			{
				kernel_time = kernel_time + n->bt;
				n->et = kernel_time;
				n->wt = kernel_time - n->at - n->bt;
				n->rt = n->wt;
				n->tt = n->et - at;
				++dispatched;
			}
			else
			{
				continue;
			}
		}
	}
}

void ps_schedule(struct list * queue, struct node ** array, int length){
	char str[5];
	uint kernel_time = 0;
	uint executing = 0;
	uint time_travel = 0;
	struct node * inExecution;
	uint estimated_finish_time = 0;

	int dispatched = 0;

	char * str1 = "No process in execution";



	for (; dispatched != length; ++kernel_time)
	{
		//printf("At kernel time: %d\n", kernel_time);
		for (int i = 0; i < length; ++i)
		{
			if ((*(array + i))->at == kernel_time)
			{
				
				priority_add(queue, *(array + i));
			}
			else
			{
				continue;
			}
		}
		//printf("Processes in Queue: \n");
		//print_list(queue);
		

		if (executing && estimated_finish_time == kernel_time)
		{
			executing = 0;
			dispatched++;
			inExecution = NULL;
		}

		if (executing == 0)
		{
			if (is_empty(queue) != 1)
			{
				inExecution = pop(queue);
				executing = 1;
				estimated_finish_time = kernel_time + inExecution->bt;
				
				inExecution->et = estimated_finish_time;
				inExecution->wt = estimated_finish_time - inExecution->at - inExecution->bt;
				inExecution->rt = inExecution->wt;
				inExecution->tt = inExecution->et - inExecution->at;
			}
		}			
		//printf("Process in execution: %s\n", (inExecution)? inExecution->name: str1 );
		//printf("Length: %d\n", length);
		//printf("Dispatched: %d\n", dispatched);
		//printf("\n\n\n");
		//fgets(str, 5, stdin);
	}	
}

void rr_schedule(struct list * queue, struct node ** array, int length, int quantum){
	char str[5];
	uint kernel_time = 0;
	uint executing = 0;
	struct node * inExecution;

	uint qm_counter = 0;

	int dispatched = 0;

	char * str1 = "No process in execution";

	for (; dispatched != length; ++kernel_time)
	{
		printf("At kernel time: %d\n", kernel_time);
		for (int i = 0; i < length; ++i)
		{
			if ((*(array + i))->at == kernel_time)
			{
				add_back(queue, *(array + i));
			}
			else
			{
				continue;
			}
		}
		//printf("Processes in Queue: \n");
		//print_list(queue);
		

		if (executing)
		{
			inExecution->workDone++;
			qm_counter++;
			if (inExecution->workDone == inExecution->bt)
			{
				printf("Termino un proceso\n");
				executing = 0;
				dispatched++;
				
				qm_counter = 0;

				/* Setting Process Properties */
				inExecution->et = kernel_time;
				inExecution->wt = kernel_time - inExecution->at - inExecution->bt;
				//inExecution->rt = inExecution->wt;
				inExecution->tt = inExecution->et - inExecution->at;
				inExecution = NULL;
			}

			if (qm_counter == quantum)
			{
				printf("/*Context Switch*/\n");
				qm_counter = 0;
				executing = 0;
				if (inExecution != NULL)
				{
					add_back(queue, inExecution);
					inExecution = NULL;
				}
				/* Switch context */

			}
		}

		if (executing == 0)
		{
			if (is_empty(queue) != 1)
			{
				inExecution = pop(queue);
				if (inExecution->workDone == 0)
				{
					inExecution->rt = kernel_time - inExecution->at;
				}
				executing = 1;
				/*				
				inExecution->et = estimated_finish_time;
				inExecution->wt = estimated_finish_time - inExecution->at - inExecution->bt;
				inExecution->rt = inExecution->wt;
				inExecution->tt = inExecution->et - inExecution->at;
				*/
			}
		}			

		printf("Processes in Queue: \n");
		print_list(queue);


		printf("Process in execution: %s\n", (inExecution)? inExecution->name: str1 );
		printf("Length: %d\n", length);
		printf("Dispatched: %d\n", dispatched);
		printf("\n\n\n");
		fgets(str, 5, stdin);
	}
}