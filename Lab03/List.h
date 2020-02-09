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

/*
void list_add_front(struct list *l, struct node *n){
	n->prev = NULL;
	n->next = &l->head;
	l->head.prev = n;
	l->head = *n;
}
*/

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
		n->next = NULL;
		return n;
	}
	
}