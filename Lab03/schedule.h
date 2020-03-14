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

	if (mode == 0)
	{
		printf("|TH\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t| \n",
			 at, bt, et, wt, rt, tt);
	}
	else if(mode == 1)
	{
		printf("|TH\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t| \n",
			 at, bt, pr, et, wt, rt, tt);
	}
	else if (mode == 2)
	{
		printf("|TH\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t| \n",
			 qu, qm, at, bt, et, wt, rt, tt);
	}
	else
	{
		printf("|TH\t|\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t|%.2f\t| \n",
			 pr, bt, et, wt, rt, tt);
	}
}

void output(struct node ** array, uint length, int mode) {
	char * separator;
	char * header;

	if (mode == 0)
	{
		separator = "|-------|-------|-------|-------|-------|-------|-------| \n";
		header = "|PXX\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n" ;
	}
	else if (mode == 1)
	{
		separator = "|-------|-------|-------|-------|-------|-------|-------|-------| \n";
		header = "|PXX\t|AT\t|BT\t|PR\t|ET\t|WT\t|RT\t|TT\t|\n";
	}
	else if (mode == 2)
	{
		separator = "|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n";
		header = "|PXX\t|QU\t|QM\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n";
	}
	else
	{
		separator = "|-------|-------|-------|-------|-------|-------|-------|-------| \n";
		header = "|PXX\t|QU\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n";
	}

	printf("%s", separator);
	printf("%s", header);
	printf("%s", separator);

	struct node * n;

	if (mode == 0)
	{
		for (int i = 0; i < length; ++i)
		{
			n = *(array + i);
			printf("|%s\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				n->name, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
			printf("%s", separator);
		}
	}
	else if (mode == 1)
	{
		for (int i = 0; i < length; ++i)
		{
			n = *(array + i);
			printf("|%s\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				n->name, n->at, n->bt, n->v, n->et, n->wt, n->rt, n->tt);
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
			printf("|%s\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				n->name, n->qu, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
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
		//printf("At kernel time: %d\n", kernel_time);
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
				//printf("Termino un proceso\n");
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
				//printf("/*Context Switch*/\n");
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

		//printf("Processes in Queue: \n");
		//print_list(queue);


		//printf("Process in execution: %s\n", (inExecution)? inExecution->name: str1 );
		//printf("Length: %d\n", length);
		//printf("Dispatched: %d\n", dispatched);
		//printf("\n\n\n");
		//fgets(str, 5, stdin);
	}
}

void mlfqs_schedule_priority(struct list * rr1, struct list * rr2, struct list * fcfs,
					struct node ** array, int length, int quantum1, int quantum2){

	uint kernel_time = 0;
	uint executing = 0;
	struct node * inExecution;
	struct node * toExecute;

	uint qm_counter1 = 0;
	uint qm_counter2 = 0;

	int dispatched = 0;
	int active_queue = 0;
	int interruption = 0;


	for (; dispatched != length; ++kernel_time)
	{
		//printf("At kernel time: %d\n", kernel_time);
		for (int i = 0; i < length; ++i)
		{
			if ((*(array + i))->at == kernel_time)
			{
				if (inExecution != NULL)
				{
					if (active_queue == 1)
					{
						if ((*(array + i))->v > inExecution->v)
						{
							interruption = 1;
							toExecute = *(array + i);
						}
						else
						{
							add_back(rr1, *(array + i));
						}

						if (toExecute != NULL && (*(array + i))->v > toExecute->v)
						{
							priority_add(rr1, toExecute);
							toExecute = *(array + i);
						}
						
					}
					else
					{
						interruption = 1;
						priority_add(rr1, *(array + i));
					}
				}
				else
				{
					priority_add(rr1, *(array + i));
				}
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

			if (active_queue == 1)
			{
				qm_counter1++;
				if (inExecution->workDone == inExecution->bt)
				{
					//printf("Termino un proceso\n");
					executing = 0;
					dispatched++;
					
					qm_counter1 = 0;

					/* Setting Process Properties */
					inExecution->et = kernel_time;
					inExecution->wt = kernel_time - inExecution->at - inExecution->bt;
					inExecution->tt = inExecution->et - inExecution->at;
					inExecution->qu = 1;
					inExecution = NULL;
				}

				if (qm_counter1 == quantum1)
				{
					//printf("/*Context Switch*/\n");
					qm_counter1 = 0;
					executing = 0;
					if (inExecution != NULL)
					{
						add_back(rr2, inExecution);
						inExecution = NULL;
					}
					/* Switch context */

				}
			}
			else if(active_queue == 2)
			{
				qm_counter2++;
				if (inExecution->workDone == inExecution->bt)
				{
					//printf("Termino un proceso\n");
					executing = 0;
					dispatched++;
					
					qm_counter2 = 0;

					/* Setting Process Properties */
					inExecution->et = kernel_time;
					inExecution->wt = kernel_time - inExecution->at - inExecution->bt;
					inExecution->tt = inExecution->et - inExecution->at;
					inExecution->qu = 2;
					inExecution = NULL;
				}

				if (qm_counter2 == quantum2)
				{
					//printf("/*Context Switch*/\n");
					qm_counter2 = 0;
					executing = 0;
					if (inExecution != NULL)
					{
						add_back(fcfs, inExecution);
						inExecution = NULL;
					}
					/* Switch context */

				}
			}
			else
			{
				if (inExecution->workDone == inExecution->bt)
				{
					//printf("Termino un proceso\n");
					executing = 0;
					dispatched++;
					
					/* Setting Process Properties */
					inExecution->et = kernel_time;
					inExecution->wt = kernel_time - inExecution->at - inExecution->bt;
					inExecution->tt = inExecution->et - inExecution->at;
					inExecution->qu = 3;
					inExecution = NULL;
				}
			}
		}

		if (interruption == 1)
		{
			interruption = 0;
			executing = 0;

			if (active_queue == 1)
			{
				add_back(rr1, inExecution);
			}
			else if (active_queue == 2)
			{
				add_back(rr2, inExecution);
			}
			else
			{
				add_back(fcfs, inExecution);
			}

			inExecution = NULL;
		}

		if (executing == 0)
		{
			if (is_empty(rr1) != 1)
			{
				inExecution = pop(rr1);
				if (inExecution->workDone == 0)
				{
					inExecution->rt = kernel_time - inExecution->at;
				}
				executing = 1;
				active_queue = 1;
				/*				
				inExecution->et = estimated_finish_time;
				inExecution->wt = estimated_finish_time - inExecution->at - inExecution->bt;
				inExecution->rt = inExecution->wt;
				inExecution->tt = inExecution->et - inExecution->at;
				*/
			}
			else
			{
				if (is_empty(rr2) != 1)
				{
					inExecution = pop(rr2);
					executing = 1;
					active_queue = 2;

				}
				else
				{
					if(is_empty(fcfs) != 1)
					{
						inExecution = pop(fcfs);
						executing = 1;
						active_queue = 3;
					}
				}
			}
		}			

		//printf("Processes in Queue: \n");
		//print_list(queue);


		//printf("Process in execution: %s\n", (inExecution)? inExecution->name: str1 );
		//printf("Length: %d\n", length);
		//printf("Dispatched: %d\n", dispatched);
		//printf("\n\n\n");
		//fgets(str, 5, stdin);
	}
}

void mlfqs_schedule_no_priority(struct list * rr1, struct list * rr2, struct list * fcfs,
					struct node ** array, int length, int quantum1, int quantum2){
	char str[5];
	char * str1 = "No process in execution";
	uint kernel_time = 0;
	uint executing = 0;
	struct node * inExecution = NULL;

	uint qm_counter1 = 0;
	uint qm_counter2 = 0;

	int dispatched = 0;
	int active_queue = 0;
	int interruption = 0;


	for (; dispatched != length; ++kernel_time)
	{
		//printf("At kernel time: %d\n", kernel_time);
		for (int i = 0; i < length; ++i)
		{
			if ((*(array + i))->at == kernel_time)
			{
				add_back(rr1, *(array + i));
				if (inExecution != NULL)
				{
					interruption = 1;
				}
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
			//printf("Process in execution: %s\n", (inExecution)? inExecution->name: str1 );
			inExecution->workDone++;

			if (active_queue == 1)
			{
				qm_counter1++;
				if (inExecution->workDone == inExecution->bt)
				{
					//printf("Termino un proceso\n");
					executing = 0;
					dispatched++;
					
					qm_counter1 = 0;

					/* Setting Process Properties */
					inExecution->et = kernel_time;
					inExecution->wt = kernel_time - inExecution->at - inExecution->bt;
					inExecution->tt = inExecution->et - inExecution->at;
					inExecution->qu = 1;
					inExecution = NULL;
				}

				if (qm_counter1 == quantum1)
				{
					//printf("/*Context Switch*/\n");
					qm_counter1 = 0;
					executing = 0;
					if (inExecution != NULL)
					{
						add_back(rr2, inExecution);
						inExecution = NULL;
					}
					/* Switch context */

				}
			}
			else if(active_queue == 2)
			{
				qm_counter2++;
				if (inExecution->workDone == inExecution->bt)
				{
					//printf("Termino un proceso\n");
					executing = 0;
					dispatched++;
					
					qm_counter2 = 0;

					/* Setting Process Properties */
					inExecution->et = kernel_time;
					inExecution->wt = kernel_time - inExecution->at - inExecution->bt;
					inExecution->tt = inExecution->et - inExecution->at;
					inExecution->qu = 2;
					inExecution = NULL;
				}

				if (qm_counter2 == quantum2)
				{
					//printf("/*Context Switch*/\n");
					qm_counter2 = 0;
					executing = 0;
					if (inExecution != NULL)
					{
						add_back(fcfs, inExecution);
						inExecution = NULL;
					}
					/* Switch context */

				}
			}
			else
			{
				if (inExecution->workDone == inExecution->bt)
				{
					//printf("Termino un proceso\n");
					executing = 0;
					dispatched++;
					
					/* Setting Process Properties */
					inExecution->et = kernel_time;
					inExecution->wt = kernel_time - inExecution->at - inExecution->bt;
					inExecution->tt = inExecution->et - inExecution->at;
					inExecution->qu = 3;
					inExecution = NULL;
				}
			}
		}

		if (interruption == 1)
		{
			interruption = 0;
			executing = 0;

			if (active_queue == 1)
			{
				if (inExecution != NULL)
				{
					add_back(rr1, inExecution);
				}
				qm_counter1 = 0;
			}
			else if (active_queue == 2)
			{
				if (inExecution != NULL)
				{
					add_back(rr2, inExecution);
				}
				qm_counter2 = 0;
			}
			else
			{
				if (inExecution != NULL)
				{
					add_back(fcfs, inExecution);
				}
				
			}

			inExecution = NULL;
		}

		if (executing == 0)
		{
			if (is_empty(rr1) != 1)
			{
				inExecution = pop(rr1);
				if (inExecution->workDone == 0)
				{
					inExecution->rt = kernel_time - inExecution->at;
				}
				executing = 1;
				active_queue = 1;
				/*				
				inExecution->et = estimated_finish_time;
				inExecution->wt = estimated_finish_time - inExecution->at - inExecution->bt;
				inExecution->rt = inExecution->wt;
				inExecution->tt = inExecution->et - inExecution->at;
				*/
			}
			else
			{
				if (is_empty(rr2) != 1)
				{
					inExecution = pop(rr2);
					executing = 1;
					active_queue = 2;

				}
				else
				{
					if(is_empty(fcfs) != 1)
					{
						inExecution = pop(fcfs);
						executing = 1;
						active_queue = 3;
					}
				}
			}
			//printf("Process to execute: %s\n", (inExecution)? inExecution->name: str1 );
		}			

		//printf("Processes in Queue: \n");
		//print_list(queue);


		
		//printf("Length: %d\n", length);
		//printf("Dispatched: %d\n", dispatched);
		//printf("\n\n\n");
		//fgets(str, 5, stdin);
	}
}