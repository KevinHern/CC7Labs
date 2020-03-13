#include <stdlib.h>
#include <stdio.h>

#define ASCENDING
#define DESCENDING
/* Abstract node that represents a disk requirement */
struct node {
	struct node * prev;
	uint ing;			// Requirement
	uint direction;
	struct node *next;
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

void print_list(struct list * l){
	struct node * n = &l->head;

	if (is_empty(l))
	{
		printf("\n");
	}
	else
	{
		while((n = n->next) != NULL){
			printf("%d", n->ing);
			if (n->next)
			{
				printf(", ");
			}
			else
			{
				printf("\n");
			}
		}
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

void add_front(struct list * l, struct node * newhead){
	newhead->next = l->head.next;
	newhead->prev = NULL;

	l->head.next->prev = newhead;

	l->head.next = newhead;

}

struct node * peek(struct list *l) {
	if (is_empty(l))
	{
		return NULL;
	}
	else
	{
		return l->head.next;
	}
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

struct node * pop_back(struct list *l){
	if (is_empty(l))
	{
		return NULL;
	}
	else
	{
		struct node * n = l->tail.prev;
		l->tail.prev = n->prev;
		l->tail.next = NULL;
		if (l->tail.prev != NULL)
		{
			l->tail.prev->next = NULL;
		}
		n->prev = NULL;
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

struct node * prev(struct node * current) {
	return current->prev;
}

struct node * next(struct node * current) {
	return current->next;
}

struct node * remove_node(struct list *l, struct node * toBeRemoved){
	if (is_empty(l))
	{
		return NULL;
	}
	else
	{
		struct node * tmp = &l->head;
		struct node * deleted = NULL;
		

		if (toBeRemoved == l->head.next)
		{

			deleted = pop(l);

		}
		else if(toBeRemoved == l->tail.prev) {

			deleted = pop_back(l);

		}
		else
		{
			while((tmp = tmp->next) != NULL){

				if (tmp == toBeRemoved) {
					tmp->prev->next = (tmp->prev) ? tmp->next : NULL;
					tmp->next->prev = (tmp->next) ? tmp->prev : NULL;
					tmp->next = NULL;
					tmp->prev = NULL;

					deleted = tmp;
				}
			}
		}

		return deleted;
	}

}

struct node * removeEngineer(struct list *l, int id) {
	if (is_empty(l))
	{
		return NULL;
	}
	else
	{
		struct node * n = &l->head;

		while((n = n->next) != NULL){
			if (n->ing == id)
			{
				break;
			}
		}

		if (n)
		{
			return remove_node(l, n);
		}
		else {
			return NULL;
		}
		
	}
}