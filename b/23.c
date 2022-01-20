#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 8

int buffer[10];
int count = 0;

sem_t semEmpty;
sem_t semFull;
pthread_mutex_t mutexBuffer;

void* producer(void* args) {
	for (int i = 0; i < 50; ++i) {
		// Produce
		int x = rand() % 100;
		sleep(1);
		// Add to the buffer
		sem_wait(&semEmpty);
		pthread_mutex_lock(&mutexBuffer);
		buffer[count++] = x;
		sem_post(&semFull);
		pthread_mutex_unlock(&mutexBuffer);
	}
	return NULL;
}

void* consumer(void* args) {
	int x;
	for (int i = 0; i < 100; ++i) {
		sem_wait(&semFull);
		pthread_mutex_lock(&mutexBuffer);
		// Remove from the buffer
		x = buffer[--count];
		pthread_mutex_unlock(&mutexBuffer);
		sem_post(&semEmpty);
		// Consume
		printf("Got %d\n", x);
//		sleep(1);
	}
	return NULL;
}


int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];

	srand(time(NULL));
	sem_init(&semEmpty, 0, 10);
	sem_init(&semFull, 0, 0);
	pthread_mutex_init(&mutexBuffer, NULL);
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (i > 0) {
			if (pthread_create(th + i, NULL, producer, NULL)) {
				perror("Failed to create thread");
			}
		} else {
			if (pthread_create(th + i, NULL, consumer, NULL)) {
				perror("Failed to create thread");
			}
		}

	}
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	pthread_mutex_destroy(&mutexBuffer);
	sem_destroy(&semEmpty);
	sem_destroy(&semFull);
	return 0;
}
