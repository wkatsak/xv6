#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "pthread.h"

#define NUM_THREADS 16
#define TARGET_COUNT_PER_THREAD 32000

uint g_counter;
pthread_mutex_t mutex;

void *thread(void *arg)
{
	int i;
	int counter;

	sleep(10);
	printf(1, "thread %d: started...\n", *(int*)arg);

	for (i=0; i<TARGET_COUNT_PER_THREAD; i++) {
		pthread_mutex_lock(&mutex);
		
		counter = g_counter;
		sleep(0);
		counter++;
		sleep(0);
		g_counter = counter;

		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(arg);
}

int main(int argc, char **argv)
{
	int i;
	int final_counter;
	int final_target = NUM_THREADS*TARGET_COUNT_PER_THREAD;

	// Initialize mutex
	if (pthread_mutex_init(&mutex, 0) < 0)
	{
		printf(1, "main: error initializing mutex...\n");
		exit();
	}

	// Initialize counter
	g_counter = 0;

	// Set up thread stuff

	// Stacks
	pthread_t threads[NUM_THREADS];
	// Args
	int *args[NUM_THREADS];

	// Allocate stacks and args and make sure we have them all
	// Bail if something fails
	for (i=0; i<NUM_THREADS; i++) {
		args[i] = (int*) malloc(sizeof(int));
		if (!args[i]) {
			printf(1, "main: could not get memory (for arg) for thread %d, exiting...\n");
			exit();
		}

		*args[i] = i;
	}

	printf(1, "main: running with %d threads...\n", NUM_THREADS);

	// Start all children
	for (i=0; i<NUM_THREADS; i++) {
		pthread_create(&threads[i], 0, thread, args[i]);
		printf(1, "main: created thread with pid %d\n", threads[i].pid);
	}
	
	// Wait for all children
	for (i=0; i<NUM_THREADS; i++) {
		void *retval;
		pthread_join(threads[i], &retval);
		printf(1, "main: thread %d joined...retval=%d\n", i, *(int*)retval);
	}

	// Check the result
	final_counter = g_counter;
	printf(1, "Final counter is %d, target is %d\n", final_counter, final_target);

	if (final_counter == final_target)
		printf(1, "TEST PASSED!\n");
	else
		printf(1, "TEST FAILED!\n");

	// Clean up mutex
	pthread_mutex_destroy(&mutex);

	// Clean up memory
	for (i=0; i<NUM_THREADS; i++) {
		free(args[i]);
	}

	// Exit
	exit();
}