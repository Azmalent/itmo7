#include <stdio.h>
#include <pthread.h>

#define PING 1
#define PONG 2

int iters = 0;
int message = 0;

void *threadPong(void *args) {
	while (1) {
		while (message != PING);
		sleep(1);
		message = PONG;
		printf("Pong! %i\n", iters++);
	}
}

void *threadPing(void *args) {
	while (1) {
		while (message != PONG);
		sleep(1);
		message = PING;
		printf("Ping! %i\n", iters++);
	}
}

int main() {
	pthread_t ping_thread, pong_thread;

	pthread_create(&ping_thread, NULL, threadPing, NULL);
	pthread_create(&pong_thread, NULL, threadPong, NULL);

	message = PONG;

	pthread_join(ping_thread, NULL);
	pthread_join(pong_thread, NULL);

	return 0;
}
