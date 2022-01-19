#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NUM 8

pthread_mutex_t mutexFuel;
pthread_mutex_t mutexWater;

int fuel = 50;
int water = 10;

void* routine(void* args) {
	if (rand() % 2 == 0) {
		pthread_mutex_lock(&mutexFuel);
		pthread_mutex_lock(&mutexWater);
	} else {
		pthread_mutex_lock(&mutexWater);
		pthread_mutex_lock(&mutexFuel);
	}
	fuel += 50;
	water = fuel;
	printf("Incremented fuel to: %d\t set water to %d\n", fuel, water);
	pthread_mutex_unlock(&mutexFuel);
	pthread_mutex_unlock(&mutexWater);
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];
	srand(time(NULL));

	pthread_mutex_init(&mutexFuel, NULL);
	pthread_mutex_init(&mutexWater, NULL);
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
	printf("Water: %d\n", fuel);
	pthread_mutex_destroy(&mutexFuel);
	pthread_mutex_destroy(&mutexWater);
	return 0;
}
