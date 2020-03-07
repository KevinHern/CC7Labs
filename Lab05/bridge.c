#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXENGS 4

/* SITUACION */
/*
Que tenemos?
	* 2 colas:
		- 1 cola para los que quieren cruzar para la derecha
		- 1 cola para los que quieren cruzar para la izquierda
	* Un maximo de 4 personas pueden cruzar el puente
	* Para asegurar que el puente no se derrumbe, se debe sincronizar a las personas
	* Garantizar que el trafico de personas sea en una sola direccion

Que necesitamos?
	- 2 colas
	- 1 variable que diga la direccion del trafico
	- 1 variable que cuente cuantos inges van pasando por el puente

Que hace un inge?
	1. Llega al puente y quiere cruzar
	2. Cruza el puente si (opciones multiples):
		* El puente se encuentra vacio
		* Hay personas cruzando en la misma direccion que quiere pero unicamente si la cantidad de personas es menor a 4.
		  No puede pasar de primero si el inge de adelante no ha salido
	3. Llega al extremo y da un signal a los demas inges que estan esperando
*/

// Variables 
int capacity = 4;
int globalDirection = 0;
int leader = 0;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_print = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition1_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition2_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t left_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t right_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t leader_cond  = PTHREAD_COND_INITIALIZER;

struct arg {
	int direction;
	int id;
	int crossTime;
};

/* ENGINEERS THREAD FUNCTIONS SECTION */

void accessBridge(struct arg * args) {
	//printf("Entra a Access Bridge\n");
	/* Make sure to check that there is enough space to cross, otherwise wait */
	pthread_mutex_lock( &condition1_mutex );
	while(capacity == 0 || (capacity < 4 && globalDirection != args->direction)) {
		if (args->direction) {
			pthread_cond_wait(&left_cond, &condition1_mutex);
		}
		else {
			pthread_cond_wait(&right_cond, &condition1_mutex);
		}
	}
	pthread_mutex_unlock( &condition1_mutex );


	pthread_mutex_lock( &mutex_1 );
	if (capacity == 4){
		leader = args->id;
	}
	--capacity;
	globalDirection = args->direction;
	pthread_mutex_unlock( &mutex_1 );
	//printf("Sale de Access Bridge\n");
}

void crossingBridge(struct arg * args) {
	//printf("Entra a Cross Bridge\n");
	for(int i = 0; i < args->crossTime; i++) {
		pthread_mutex_lock( &mutex_print );
		if(leader == args->id) {
			printf("(LEADER) ");
		}
		printf("Engineer %d is crossing the bridge ", args->id);
		if (args->direction){
			printf("to the RIGHT\n");
		}
		else {
			printf("to the LEFT\n");
		}
		
		pthread_mutex_unlock( &mutex_print );
		sleep(1);
	}
	//printf("Sale de Cross Bridge\n");
}

void exitBridge(struct arg * args) {
	pthread_mutex_lock( &condition2_mutex );
	while(leader != 0 && leader != args->id) {
		pthread_mutex_lock( &mutex_print );

		printf("Engineer %d is crossing the bridge ", args->id);
		if (args->direction){
			printf("to the RIGHT behind the leader\n");
		}
		else {
			printf("to the LEFT behind the leader\n");
		}

		pthread_mutex_unlock( &mutex_print );
		pthread_cond_wait(&leader_cond, &condition2_mutex);
	}
	pthread_mutex_lock( &mutex_2 );
	leader = args->id;
	pthread_mutex_unlock( &mutex_2 );
	pthread_mutex_unlock( &condition2_mutex );

	pthread_mutex_lock( &mutex_2 );
	leader = 0;
	printf("(LEADER) Engineer %d finished crossing the bridge\n", args->id);
	++capacity;
	pthread_cond_broadcast( &leader_cond );
	pthread_mutex_unlock( &mutex_2 );
	pthread_cond_broadcast( &left_cond );
	pthread_cond_broadcast( &right_cond );
}

// Function to be executed by every thread
/* PSEUDOCODE PROCESS
	accessBridge(* thread);

	crossingBridge(randomTime);

	exitBridge();

*/

void * crossBridge(void * arg) {
	struct arg * args = (struct arg *)arg;
	sleep(rand() % 10);
	printf("Engineer %d arrived to the bridge\n", args->id);

	accessBridge(args);

	//printf("\nTransicion\n\n");
	crossingBridge(args);
	exitBridge(args);



	return NULL;
}

/* END OF FUNCTIONS SECTION */


int read_requirement(){
	int tmp;
	printf("How many engineers do you want to cross the bridge?: ");
	scanf("%d", &tmp);
	printf("\n\n");
	return tmp;
}

int main(int argc, char const *argv[])
{
	/* Set and Initialize Random Number Generator */
	time_t t;
	srand((unsigned) time(&t));

	// Read number of engineers
	int engineers = read_requirement();

	// Set necessary arrays
	pthread_t threads[engineers];
	struct arg ** args = malloc(engineers*sizeof(struct arg));

	for (int i = 0; i < engineers; i++)
	{
		// SET ARGUMENTS
		struct arg * argi = malloc(sizeof(struct arg));
		argi->direction = rand() % 2;
		argi->id = i + 1;
		argi->crossTime = (rand() % 3)+1;

		*(args + i) = argi;
		
		// Create Thread
		pthread_t tid;
		pthread_create(&threads[i], NULL, crossBridge, argi);
	}
		

	// MAIN THREAD WAITS FOR EVERYONE ELSE
	for (int i = 0; i < engineers; i++)
		pthread_join(threads[i], NULL);

	// HOUSEKEEPING
	for (int i = 0; i < engineers; ++i)
	{
		free(*(args + i));
	}
	free(args);
	return 0;
}