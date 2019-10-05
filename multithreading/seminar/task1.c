#include <stdio.h>
#include <pthread.h>

#define ITER_COUNT 10
#define THREAD_COUNT 3

void *threadTask(void *args) {
	int i;
	int thread_id = pthread_self();
	for (i = 0; i < ITER_COUNT; i++) {
		printf("Thread №%i, iteration %i\n", thread_id, i);
	}
}

int main() {
	int i;
	pthread_t threads[THREAD_COUNT];

	for (i = 0; i < THREAD_COUNT; i++) {
		pthread_create(&threads[i], NULL, threadTask, NULL);
	}

	for (i = 0; i < THREAD_COUNT; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("\nPress any key to exit\n");
	getchar();

	return 0;
}
