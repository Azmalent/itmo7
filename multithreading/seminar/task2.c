#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

char filename[25]; 

void *threadTask(void *args) {
	FILE *file;
	char c;

	while (1) {
		file = fopen(filename, "r");
		if (file != NULL) {
			printf("Successfully opened file!\n") ;
			while ((c = fgetc(file)) != EOF);
		} else {
			printf("Failed to open file!\n") ;
		}
		sleep(1);
	}
}

int main() {
	pthread_t thread;
	
	printf("Input the filename (max 25 chars): \n") ;
	gets(filename);

	pthread_create(&thread, NULL, threadTask, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_cancel(thread);
	pthread_join(thread, NULL);

	return 0;
}
