#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int fuel = 0;
pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;

void* fill_fuel(void* arg) {
	for (int i = 0; i < 5; ++i) {
		pthread_mutex_lock(&mutexFuel);
		fuel += 15;
		printf("Filled fuel... %d\n", fuel);
		pthread_mutex_unlock(&mutexFuel);
		pthread_cond_signal(&condFuel);
		sleep(1);
	}
	return NULL;
}

void* fill_car(void* arg) {
	pthread_mutex_lock(&mutexFuel);
	while (fuel < 40) {
		printf("No fuel. Waiting...\n");
		pthread_cond_wait(&condFuel, &mutexFuel);
	}
	fuel -= 40;
	pthread_mutex_unlock(&mutexFuel);
	printf("Got fuel. Fuel left: %d\n", fuel);
	return NULL;
}


int main(int argc, char* argv[]) {
	pthread_t th[2];
	pthread_mutex_init(&mutexFuel, NULL);
	pthread_cond_init(&condFuel, NULL);
	for (int i = 0; i < 2; ++i) {
		if (i == 1) {
			if (pthread_create(th + i, NULL, fill_fuel, NULL)) {
				perror("Failed to create a thread");
			}
		} else {
			if (pthread_create(th + i, NULL, fill_car, NULL)) {
				perror("Failed to create a thread");
			}
		}

	}
	for (int i = 0; i < 2; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	pthread_mutex_destroy(&mutexFuel);
	pthread_cond_destroy(&condFuel);
	return 0;
}
