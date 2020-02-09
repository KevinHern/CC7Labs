#include <stdlib.h>
#include "List.h"

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
				++dispatched;
			}
			else
			{
				continue;
			}
		}
		//dispatched++;
	}

}