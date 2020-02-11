#include <stdlib.h>

struct node {
	struct node * prev;
	int at;			//What time the process will come
	int bt;			//Execution time in CPU
	int v;			//Priority
	int qu;			//queue
	int qm;			//quantum
	int et;			//exit time
	int wt;			//waiting time
	int rt;			//response time
	int tt;			//turnaround time
	uint workDone;
	struct node *next;
	char * name;
};

//Double linked list
struct list {
	struct node head;
	struct node tail;
};

void init_list(struct list *l) {
	l->head.prev = NULL;
	l->head.next = NULL;
	l->tail.prev = NULL;
	l->tail.next = NULL;
}

int is_empty(struct list *l) {
	if(l->head.next == NULL){
		return 1;
	}
	else{
		return 0;
	}
}


uint length(struct list *l){
	uint i = 0;
	struct node * tmp = &l->head;

	if(is_empty(l)){
		return i;
	}
	else{
		for (; tmp->next != NULL; ++i)
		{
			tmp = tmp->next;
		}

		return i;
	}
}


void add_back(struct list *l, struct node *n){
	if (is_empty(l))
	{
		n->prev = NULL;
		n->next = NULL;
		l->head.next = n;
		l->tail.prev = n;
	}
	else
	{
		l->tail.prev->next = n;

		n->prev = l->tail.prev;
		n->next = NULL;
		l->tail.prev = n;
		l->tail.next = NULL;
	}
}

void insert(struct list * pq, struct node * newnode, struct node * reference){
	newnode->next = reference;
	newnode->prev = reference->prev;

	if (reference->prev == NULL)
	{
		pq->head.next = newnode;
	}
	else
	{
		reference->prev->next = newnode;
	}

	reference->prev = newnode;

}

void reschedule(struct list *pq, struct node * newnode, struct node * threshold) {
	if (threshold)
	{
		insert(pq, newnode, threshold);
	}
	else
	{

		add_back(pq, newnode);
	}
}

void priority_add(struct list *pq, struct node *n){
	
	struct node * dog = &pq->head;
	if (is_empty(pq))
	{
		add_back(pq, n);
	}
	else
	{	
		while((dog = dog->next) != NULL){
			if (dog->v > n->v)
			{
				reschedule(pq, n, dog);
				break;
			}
			else
			{
				if (dog->next == NULL)
				{

					reschedule(pq, n, NULL);
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}	
}

void add_front(struct list * l, struct node * newhead){
	newhead->next = l->head.next;
	newhead->prev = NULL;

	l->head.next->prev = newhead;

	l->head.next = newhead;

}


struct node * pop(struct list *l){
	if (is_empty(l))
	{
		return NULL;
	}
	else
	{
		struct node * n = l->head.next;
		l->head.next = n->next;
		l->head.prev = NULL;
		if (l->head.next != NULL)
		{
			l->head.next->prev = NULL;
		}
		n->next = NULL;
		return n;
	}
	
}

struct node * get_at(struct list *l, int index){
	if (is_empty(l))
	{
		return NULL;
	}
	else if(length(l) - 1 < index)
	{
		return NULL;
	}
	else
	{
		struct node * n = &l->head;

		for (int i = 0; i <= index; ++i)
		{
			n = n->next;
		}
		return n;
	}
	
}

void print_list(struct list * l){
	struct node * n = &l->head;

	if (is_empty(l))
	{
		printf("List is empty\n");
	}
	else
	{
		while((n = n->next) != NULL){
			printf("Proceso %s\n", n->name);
		}
	}
}