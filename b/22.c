#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 16

sem_t semaphore;

void* routine(void* args) {
	printf("(%d) Waiting in the logging queue\n", *(int*)args);
	sem_wait(&semaphore);
	printf("(%d) Logged in\n", *(int*)args);
	sleep(rand() % 5 + 1);
	printf("(%d) Logged out\n", *(int*)args);
	sem_post(&semaphore);
	free(args);
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];

	srand(time(NULL));
	sem_init(&semaphore, 0, 6);
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
