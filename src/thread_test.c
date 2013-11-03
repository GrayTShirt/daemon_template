#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define NUM_THREADS 100

void *TaskCode(void *argument) {
	int tid;

	time_t current_time;
	current_time = time(NULL);
	tid = *((int *) argument);
	printf("Hello World! It's me, thread %d!, my time is: %d\n", tid, (int) current_time);

	/* optionally: insert more useful stuff here */

	return NULL;
}

int main(void) {
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];
	int rc, i;

	/* create all threads */
	for (i=NUM_THREADS - 1; i >= 0; --i) {
		thread_args[i] = i;
		printf("In main: creating thread %d\n", i);
		rc = pthread_create(&threads[i], NULL, TaskCode, (void *) &thread_args[i]);
		assert(0 == rc);
	}

	/* wait for all threads to complete */
	for (i=0; i<NUM_THREADS; ++i) {
		rc = pthread_join(threads[i], NULL);
		assert(0 == rc);
	}

	printf("\n\nExiting\n");
	exit(EXIT_SUCCESS);
}
