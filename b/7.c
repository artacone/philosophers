#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
	int value = *(int*)arg;
	printf("%d ", value);
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[10];
	for (int i = 0; i < 10; ++i) {
		if (pthread_create(th + i, NULL, &routine, primes + i)) {
			perror("Failed to create a thread");
		}
	}
	for (int i = 0; i < 10; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	return 0;
}
