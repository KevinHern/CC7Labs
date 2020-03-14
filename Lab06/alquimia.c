#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "list.h"
/* SITUACION */
/*
Que tenemos?

*/

// Variables 
int success = 0;
int hydrogen_molecules = 0;
int nitrogen_molecules = 0;
int oxygen_molecules = 0;
int awakener = 0;
int used_molecules = 0;
int terminateReaction = 0;

int H = 0;
int N = 0;
int O = 0;
int O2 = 0;
int NH3 = 0;
int reacting = 0;

// Mutexes
pthread_mutex_t hydrogen_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t nitrogen_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t NH3_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t oxygen_mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition Mutexes
pthread_mutex_t reaction_condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reacting_condition_mutex = PTHREAD_MUTEX_INITIALIZER;

// Extra Mutexs
pthread_mutex_t mutex_print = PTHREAD_MUTEX_INITIALIZER;	

// Condition Variables
pthread_cond_t hydrogen_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t nitrogen_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t oxygen_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t reaction_cond  = PTHREAD_COND_INITIALIZER;	
pthread_cond_t reacting_cond  = PTHREAD_COND_INITIALIZER;

// Structure that represents arguments

struct list hydrogenQueue;
struct list nitrogenQueue;
struct list oxygenQueue;
struct list threads;

void reactingSection(pthread_cond_t * cond_var) {
	printf("Entering Reaction Section\n");
	pthread_mutex_lock( &reaction_condition_mutex );		
	if((O2 < 1 && NH3 < 1) || reacting){
		printf("Entering Reaction Cond Var\n");
		pthread_cond_wait( cond_var, &reaction_condition_mutex );
	}
	else {
		pthread_mutex_lock( &mutex_print );			
		reacting = 1;
		printf("# IMPORTANTE EVENT #> FIRE!!!!!!!\n");
		pthread_mutex_unlock( &mutex_print );	
	}
	pthread_mutex_unlock( &reaction_condition_mutex );	
	printf("Exiting Reaction Section\n");
}

void reset() {
	++used_molecules;
	if (used_molecules == 7)
	{
		pthread_mutex_lock( &reaction_condition_mutex );		
		reacting = 0;
		pthread_mutex_unlock( &reaction_condition_mutex );		
		used_molecules = 0;
		awakener = 0;
	}
}

void awake(int oxy, int nit, int hyd) {
	pthread_mutex_lock( &reaction_condition_mutex );
	--O2;
	--NH3;
	pthread_mutex_unlock( &reaction_condition_mutex );

	for (int i = 0; i < oxy; ++i)
	{
		pthread_cond_signal( &oxygen_cond );
	}
	for (int i = 0; i < nit; ++i)
	{
		pthread_cond_signal( &nitrogen_cond );
	}
	for (int i = 0; i < hyd; ++i)
	{
		pthread_cond_signal( &hydrogen_cond );
	}
}

/* MOLECULES THREAD FUNCTIONS SECTION */
void * Ox(void * arg) {
	sleep((rand()%2) + 1);
	struct node * thread = (struct node *)arg;

	pthread_mutex_lock( &mutex_print );												// Print Mutex Acquire
	printf("# CREATION #> Creating (id%d) Oxygen Molecule\n", thread->id);
	pthread_mutex_unlock( &mutex_print );		

	// Oxygen Molecule Section
	pthread_mutex_lock( &oxygen_mutex );											// Oxygen Mutex Acquire

										// Print Mutex Release

	if ((oxygen_molecules % 2) == 0)
	{
		pthread_mutex_lock( &reaction_condition_mutex );		
		pthread_mutex_lock( &mutex_print );	
		printf("# MOLECULE CREATION #> An O2 Molecule has been made!!!!!!!\n");
		++O2;
		pthread_mutex_unlock( &mutex_print );
		pthread_mutex_unlock( &reaction_condition_mutex );					
											
	}
	pthread_mutex_unlock( &oxygen_mutex );											// Oxygen Mutex Release



	// Reaction Section
	reactingSection(&oxygen_cond);


	// Reacting Section
	pthread_mutex_lock( &reacting_condition_mutex );	

	// If you are the first molecule, awake everyone else
	if(awakener == 0) {
		// Awake everyone 
		awake(1, 1, 3);
		awakener = 1;
	}

	pthread_mutex_lock( &mutex_print );		
	printf("> (id%d) Oxygen Molecule\n", thread->id);
	pthread_mutex_unlock( &mutex_print );	

	// Reset Values
	reset();
	
	pthread_mutex_unlock( &reacting_condition_mutex );	
	free(thread);	
}

void * Ni(void * arg) {
	sleep((rand()%2) + 1);
	struct node * thread = (struct node *)arg;

	pthread_mutex_lock( &mutex_print );											
	printf("# MOLECULE CREATION #> Creating (id%d) Nitrogen Molecule\n", thread->id);
	pthread_mutex_unlock( &mutex_print );	

	// Oxygen Molecule Section
	pthread_mutex_lock( &nitrogen_mutex );																									
	
	pthread_mutex_lock( &NH3_mutex );
	++N;											
	if(N > 0 && H > 2) {
		H = H -3;
		--N;
		
		pthread_mutex_lock( &reaction_condition_mutex );
		pthread_mutex_lock( &mutex_print );											// Print Mutex Acquire
		printf("# MOLECULE CREATION #> A NH3 Molecule has been made!!!!!!!\n");
		++NH3;
		pthread_mutex_unlock( &mutex_print );										
		pthread_mutex_unlock( &reaction_condition_mutex );
	}
	pthread_mutex_unlock( &NH3_mutex );	
						
	pthread_mutex_unlock( &nitrogen_mutex );											// Oxygen Mutex Release

	// Reaction Section
	reactingSection(&nitrogen_cond);


	// Reacting Section
	pthread_mutex_lock( &reacting_condition_mutex );	

	// If you are the first molecule, awake everyone else
	if(awakener == 0) {
		// Awake everyone 
		awake(2, 0, 3);
		awakener = 1;
	}

	pthread_mutex_lock( &mutex_print );		
	printf("> (id%d) Nitrogen Molecule\n", thread->id);
	pthread_mutex_unlock( &mutex_print );	

	// Reset Section
	reset();
	
	pthread_mutex_unlock( &reacting_condition_mutex );	
	free(thread);	
}

void * Hi(void * arg) {
	sleep((rand()%2) + 1);
	struct node * thread = (struct node *)arg;

	pthread_mutex_lock( &mutex_print );											
	printf("# CREATION #> Creating (id%d) Hydrogen Molecule\n", thread->id);
	pthread_mutex_unlock( &mutex_print );	

	// Oxygen Molecule Section
	pthread_mutex_lock( &hydrogen_mutex );																								
	
	pthread_mutex_lock( &NH3_mutex );
	++H;											
	if(N > 0 && H > 2) {
		H = H -3;
		--N;
		
		pthread_mutex_lock( &reaction_condition_mutex );
		pthread_mutex_lock( &mutex_print );											// Print Mutex Acquire
		printf("# MOLECULE CREATION #> A NH3 Molecule has been made!!!!!!!\n");
		++NH3;
		pthread_mutex_unlock( &mutex_print );										
		pthread_mutex_unlock( &reaction_condition_mutex );
	}
	pthread_mutex_unlock( &NH3_mutex );	
						
	pthread_mutex_unlock( &hydrogen_mutex );											// Oxygen Mutex Release

	// Reaction Section
	reactingSection(&hydrogen_cond);


	// Reacting Section
	pthread_mutex_lock( &reacting_condition_mutex );	

	// If you are the first molecule, awake everyone else
	if(awakener == 0) {
		// Awake everyone 
		awake(2, 1, 2);
		awakener = 1;
	}

	pthread_mutex_lock( &mutex_print );		
	printf("> (id%d) Hydrogen Molecule\n", thread->id);
	pthread_mutex_unlock( &mutex_print );	

	// Reset Section
	reset();
	
	pthread_mutex_unlock( &reacting_condition_mutex );	
	free(thread);	
}

/* END OF FUNCTIONS SECTION */


int read_requirement(){
	int tmp;
	printf("How many reactions do you wish to do?: ");
	scanf("%d", &tmp);
	printf("\n\n");
	return tmp;
}

void setMolecules(int number) {
	hydrogen_molecules = 3*number;
	nitrogen_molecules = number;
	oxygen_molecules = 2*number;
}

int main(int argc, char const *argv[])
{
	/* Set and Initialize Lists  */
	init_list(&hydrogenQueue);
	init_list(&nitrogenQueue);
	init_list(&oxygenQueue);

	// Read number of reactions
	int reactions = read_requirement();
	setMolecules(reactions);
	printf("# Threads: %d\n", 6*reactions);

	// Set necessary arrays
	pthread_t threads[6*reactions];

	for (int i = 0; i < (6*reactions); i++)
	{
		printf("Iteration: %d\n", i);
		int next = 0;
		// Sleep for 1-4 seconds
		//

		struct node * thread = malloc(sizeof(struct node));

		thread->tid = threads[i];
		thread->id = i+1;

		/* Elements:
			0: Hydrogen
			1: Nitrogen
			2: Oxygen
		*/
		thread->element = rand() % 3;
		printf("Type: %d\n", thread->element);

		pthread_mutex_lock( &oxygen_mutex );
		if(thread->element == 2 && oxygen_molecules != 0) {
			// Oxygen
			--oxygen_molecules;
			printf("Creating Oxygen\n");
			pthread_mutex_unlock( &oxygen_mutex );
			pthread_create(&threads[i], NULL, Ox, thread);
			//continue;
		}
		pthread_mutex_unlock( &oxygen_mutex );

		pthread_mutex_lock( &nitrogen_mutex );
		if(thread->element == 1 && nitrogen_molecules != 0) {
			// Nitrogen
			--nitrogen_molecules;
			printf("Creating Nitrogen\n");
			pthread_mutex_unlock( &nitrogen_mutex );
			pthread_create(&threads[i], NULL, Ni, thread);
			//continue;
		}
		pthread_mutex_unlock( &nitrogen_mutex );

		pthread_mutex_lock( &hydrogen_mutex );
		if(thread->element == 0 && hydrogen_molecules != 0){
			// Hydrogen
			--hydrogen_molecules;
			printf("Creating Hydrogen\n");
			pthread_mutex_unlock( &hydrogen_mutex );
			pthread_create(&threads[i], NULL, Hi, thread);
			//continue;
		}
		pthread_mutex_unlock( &hydrogen_mutex );
	}

	for (int i = 0; i < (6*reactions); ++i)
	{
		pthread_join(threads[i], NULL);
	}
	return 0;
}