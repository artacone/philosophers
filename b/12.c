#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// chefs = threads
// stove = shared data (+mutex)

pthread_mutex_t mutexesStove[4];
int stoveFuel[4] = { 100, 100, 100, 100 };

void* routine(void* arg) {
	for (int i = 0; i < 4; ++i) {
		if (pthread_mutex_trylock(mutexesStove + i) == 0) {
			int fuelNeeded = (rand() % 20);
			if (fuelNeeded > stoveFuel[i]) {
				printf("No more fuel... going home\n");
			} else {
				stoveFuel[i] -= fuelNeeded;
				usleep(500000);
				printf("Fuel left %d\n", stoveFuel[i]);
			}
			pthread_mutex_unlock(mutexesStove + i);
			break;
		} else {
			if (i == 3) {
				printf("No stove available yet! Waiting...\n");
				usleep(300000);
				i = 0;
			}
		}
	}
	return NULL;
}


int main(int argc, char* argv[]) {
	pthread_t th[10];
	srand(time(NULL));
	for (int i = 0; i < 4; ++i) {
		pthread_mutex_init(mutexesStove + i, NULL);
	}
	for (int i = 0; i < 10; ++i) {
		if (pthread_create(th + i, NULL, routine, NULL)) {
			perror("Failed to create a thread");
		}
	}
	for (int i = 0; i < 10; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	for (int i = 0; i < 4; ++i) {
		pthread_mutex_destroy(mutexesStove + i);
	}
	return 0;
}
