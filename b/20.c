#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NUM 8

pthread_mutex_t mutexFuel;

int fuel = 50;

void* routine(void* args) {
	pthread_mutex_lock(&mutexFuel);
	pthread_mutex_lock(&mutexFuel);
	fuel += 50;
	printf("Incremented fuel to: %d\n", fuel);
	pthread_mutex_unlock(&mutexFuel);
	pthread_mutex_unlock(&mutexFuel);
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];
	pthread_mutexattr_t recursiveMutexAttr;

	pthread_mutexattr_init(&recursiveMutexAttr);
	pthread_mutexattr_settype(&recursiveMutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutexFuel, &recursiveMutexAttr);
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_create(th + i, NULL, routine, NULL)) {
			perror("Failed to create thread");
		}
	}
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	printf("Fuel: %d\n", fuel);
	pthread_mutexattr_destroy(&recursiveMutexAttr);
	pthread_mutex_destroy(&mutexFuel);
	return 0;
}
