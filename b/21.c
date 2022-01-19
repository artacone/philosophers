#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 8

sem_t semaphore;

void* routine(void* args) {
	sem_wait(&semaphore);
	sleep(1);
	printf("Hello from thread %d\n", *(int*)args);
	sem_post(&semaphore);
	free(args);
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];

	sem_init(&semaphore, 0, 4);
	for (int i = 0; i < THREAD_NUM; ++i) {
		int *a = malloc(sizeof *a);
		*a = i;
		if (pthread_create(th + i, NULL, routine, a)) {
			perror("Failed to create thread");
		}
	}
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	sem_destroy(&semaphore);
	return 0;
}
