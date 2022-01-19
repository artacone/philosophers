#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex;

void* routine(void* arg) {
	if (pthread_mutex_trylock(&mutex) == 0) {
		printf("Got locked\n");
		sleep(1);
		pthread_mutex_unlock(&mutex);
	} else {
		printf("Didn't get lock\n");
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[4];
	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < 4; ++i) {
		if (pthread_create(th + i, NULL, &routine, NULL)) {
			perror("Failed to create a thread");
		}
	}
	for (int i = 0; i < 4; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	return 0;
}
