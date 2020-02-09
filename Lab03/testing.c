#include <stdio.h>
#include <stdlib.h>
#include "List.h"

int main(int argc, char const *argv[])
{
	struct list lista;

	init_list(&lista);

	struct node n1;
	n1.prev = NULL;
	n1.next = NULL;
	n1.arrivalTime = 100;

	struct node n2;
	n2.prev = NULL;
	n2.next = NULL;
	n2.arrivalTime = 200;

	printf("%d\n", length(&lista));
	list_add_queue(&lista, &n1);
	printf("%d\n", length(&lista));
	list_add_queue(&lista, &n2);
	printf("%d\n", length(&lista));

	struct node * temp = pop(&lista);
	printf("%d\n", temp->arrivalTime);

	temp = pop(&lista);
	printf("%d\n", temp->arrivalTime);
	printf("%d\n", length(&lista));
	return 0;
}