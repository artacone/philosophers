#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NUM 2

pthread_mutex_t mutexFuel = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condFuel = PTHREAD_COND_INITIALIZER;

void* routine(void* args) {
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];

	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_create(th + i, &detachedThread, routine, NULL)) {
			perror("Failed to create thread");
		}
	}
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	pthread_mutex_destroy(&mutexFuel);
	return 0;
}
