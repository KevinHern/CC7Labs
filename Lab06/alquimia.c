#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "list.h"
/* SITUACION */
/*
Que tenemos?

*/

// Variables 
int used_molecules = 0;
int oxygen_waiters = 0;
int nitrogen_waiters = 0;
int hydrogen_waiters = 0;

int hydrogen_molecules = 0;
int nitrogen_molecules = 0;
int oxygen_molecules = 0;


int H = 0;
int N = 0;
int O = 0;
int reactionInProgress = 0;
int reactions = 0;

// Mutexes
pthread_mutex_t O2_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t NH3_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t used_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reaction_mutex = PTHREAD_MUTEX_INITIALIZER;

// Extra Mutexs
pthread_mutex_t mutex_print = PTHREAD_MUTEX_INITIALIZER;	

// Condition Variables
pthread_cond_t O2_cond  = PTHREAD_COND_INITIALIZER;	
pthread_cond_t N_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t H3_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t hydrogen_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t nitrogen_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t oxygen_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t waiters = PTHREAD_COND_INITIALIZER;

/* USEFUL FUNCTIONS */

void awake(int oxy, int nit, int hyd) {
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

void reset(struct node * thread) {
	pthread_mutex_lock( &used_mutex );
	++used_molecules;
	if (used_molecules == 6)
	{
		used_molecules = 0;
		pthread_mutex_lock( &mutex_print );	
		printf("# REACTION %d FINISHED #> Reaction finished successfuly. We converted Acid into... Water?\n", reactions);
		pthread_mutex_unlock( &mutex_print );

		pthread_mutex_lock( &reaction_mutex );	
		reactionInProgress = 0;
		pthread_mutex_unlock( &reaction_mutex );

		pthread_cond_broadcast ( &waiters );
	}

	pthread_mutex_unlock( &used_mutex );

	free(thread);
}

void criticalSection(pthread_cond_t * cond_var, int oxy, int nit, int hyd, struct node *thread) {
	
	/*
	Critical Section:
		Is a reaction occurring?
			> Yes: Wait untill reaction finishes
			> No: Hay suficientes moleculas?
				> Yes: Reaction
					> Only allow threads in the queues to execute
				> No: Wait untill enough molecules exist
	*/
	//printf("Gotta sleep Thread %d\n", thread->id);

	pthread_mutex_lock( &reaction_mutex );

	if(reactionInProgress) {
		//printf("Gotta sleep Thread %d\n", thread->id);
		pthread_cond_wait( &waiters, &reaction_mutex );
	}

	//printf("What Thread is this?: Thread %d\n", thread->id);
	switch(thread->element) {
		case 0:
		++hydrogen_waiters;
		break;

		case 1:
		++nitrogen_waiters;
		break;

		case 2:
		++oxygen_waiters;
		break;
	}

	if (oxygen_waiters < 2 || nitrogen_waiters < 1 || hydrogen_waiters < 3)
	{
		//printf("Waiting to complete reaction Thread %d\n", thread->id);
		pthread_cond_wait( cond_var, &reaction_mutex );
	}
	else {
		reactionInProgress = 1;
		oxygen_waiters -= 2;
		nitrogen_waiters -= 1;
		hydrogen_waiters -= 3;
		pthread_mutex_lock( &mutex_print );	
		printf("# IMPORTANT EVENT #> FIRE!!!!!!!\n");
		printf("\t###\tEPIC REACTION # %d\t###\n", ++reactions);
		pthread_mutex_unlock( &mutex_print );
		awake(oxy, nit, hyd);
	}

	if (thread->element == 2)
	{
		pthread_mutex_lock( &mutex_print );		
		printf("(REACTION %d)> (id%d) Oxygen Molecule\n", reactions, thread->id);
		pthread_mutex_unlock( &mutex_print );	
	}
	else if(thread->element == 1) {
		pthread_mutex_lock( &mutex_print );		
		printf("(REACTION %d)> (id%d) Nitrogen Molecule\n", reactions, thread->id);
		pthread_mutex_unlock( &mutex_print );	
	}
	else if(thread->element == 0) {
		pthread_mutex_lock( &mutex_print );		
		printf("(REACTION %d)> (id%d) Hydrogen Molecule\n", reactions, thread->id);
		pthread_mutex_unlock( &mutex_print );
	}

	pthread_mutex_unlock( &reaction_mutex );

	reset(thread);
}

/* MOLECULES THREAD FUNCTIONS SECTION */
void * Ox(void * arg) {
	//sleep((rand()%2) + 1);
	struct node * thread = (struct node *)arg;


	pthread_mutex_lock( &O2_mutex );

	pthread_mutex_lock( &mutex_print );
	printf("# CREATION #> Creating (id%d) Oxygen Molecule\n", thread->id);
	++O;
	pthread_mutex_unlock( &mutex_print );
	if(O != 2) {
		pthread_cond_wait( &O2_cond, &O2_mutex );
	}
	else {
		pthread_mutex_lock( &mutex_print );
		printf("# MOLECULE CREATION #> An O2 Molecule has been made!!!!!!!\n");
		O = 0;
		pthread_mutex_unlock( &mutex_print );
		pthread_cond_signal ( &O2_cond );
	}

	//printf("Must be Oxygen: Thread %d\n", thread->id);
	
	pthread_mutex_unlock( &O2_mutex );

	
	criticalSection(&oxygen_cond, 1, 1, 3, thread);

}

void * Ni(void * arg) {
	//sleep((rand()%2) + 1);
	struct node * thread = (struct node *)arg;


	pthread_mutex_lock( &NH3_mutex );

	pthread_mutex_lock( &mutex_print );
	printf("# CREATION #> Creating (id%d) Nitrogen Molecule\n", thread->id);
	++N;
	pthread_mutex_unlock( &mutex_print );
	if(H < 3) {
		pthread_cond_wait( &N_cond, &NH3_mutex );
	}
	else {
		pthread_mutex_lock( &mutex_print );
		printf("# MOLECULE CREATION #> An NH3 Molecule has been made!!!!!!!\n");
		--N;
		H = H-3;
		//printf("N: %d\n", N);
		//printf("H: %d\n", H);
		pthread_mutex_unlock( &mutex_print );
		pthread_cond_signal ( &H3_cond );
		pthread_cond_signal ( &H3_cond );
		pthread_cond_signal ( &H3_cond );
	}
	//printf("Must be Nitrogen: Thread %d\n", thread->id);
	pthread_mutex_unlock( &NH3_mutex );

	
	criticalSection(&nitrogen_cond, 2, 0, 3, thread);
}

void * Hi(void * arg) {
	//sleep((rand()%2) + 1);
	struct node * thread = (struct node *)arg;


	pthread_mutex_lock( &NH3_mutex );

	pthread_mutex_lock( &mutex_print );
	printf("# CREATION #> Creating (id%d) Hydrogen Molecule\n", thread->id);
	++H;
	pthread_mutex_unlock( &mutex_print );
	if(N < 1 || H < 3) {
		pthread_cond_wait( &H3_cond, &NH3_mutex );
	}
	else {
		pthread_mutex_lock( &mutex_print );
		printf("# MOLECULE CREATION #> An NH3 Molecule has been made!!!!!!!\n");
		--N;
		H = H-3;
		//printf("N: %d\n", N);
		//printf("H: %d\n", H);
		pthread_mutex_unlock( &mutex_print );
		pthread_cond_signal ( &N_cond );
		pthread_cond_signal ( &H3_cond );
		pthread_cond_signal ( &H3_cond );

	}
	//printf("Must be Hydrogen: Thread %d\n", thread->id);
	pthread_mutex_unlock( &NH3_mutex );

	
	criticalSection(&hydrogen_cond, 2, 1, 2, thread);
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
	printf("NH3 + O2 → Water?\n");


	// Read number of reactions
	int reactions = read_requirement();
	setMolecules(reactions);

	printf("A total of %d Nitrogen, %d Hydrogen and %d Oxygen molecules are going to be created...\n", 
		nitrogen_molecules, hydrogen_molecules, oxygen_molecules);
	

	// Set necessary arrays
	pthread_t threads[6*reactions];

	for (int i = 0; i < (6*reactions); )
	{
		int old_i = i;
		//printf("Iteration: %d\n", i);
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
		//printf("Type: %d\n", thread->element);

		if(thread->element == 2 && oxygen_molecules != 0) {
			sleep((rand()%3) + 1);
			// Oxygen
			--oxygen_molecules;
			//printf("Creating Oxygen\n");
			pthread_create(&threads[i], NULL, Ox, thread);
			++i;
			//continue;
		}


		if(thread->element == 1 && nitrogen_molecules != 0) {
			sleep((rand()%3) + 1);
			// Nitrogen
			--nitrogen_molecules;
			//printf("Creating Nitrogen\n");
			pthread_create(&threads[i], NULL, Ni, thread);
			++i;
			//continue;
		}

		
		if(thread->element == 0 && hydrogen_molecules != 0){
			sleep((rand()%3) + 1);
			// Hydrogen
			--hydrogen_molecules;
			//printf("Creating Hydrogen\n");
			pthread_create(&threads[i], NULL, Hi, thread);
			++i;
			//continue;
		}


		if (old_i == i)
		{
			free(thread);
		}
	}

	for (int i = 0; i < (6*reactions); ++i)
	{
		pthread_join(threads[i], NULL);
	}
	return 0;
}