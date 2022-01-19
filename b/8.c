#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
	int index = *(int*)arg;
	int sum = 0;
	for (int i = 0; i < 5; ++i) {
		sum += primes[index + i];
	}
	printf("Local sum: %d\n", sum);
	*(int*)arg = sum;
	return (void*)arg;
}

int main(int argc, char* argv[]) {
	pthread_t th[2];
	for (int i = 0; i < 2; ++i) {
		int* a = malloc(sizeof *a);
		*a = i * 5;
		if (pthread_create(th + i, NULL, &routine, a)) {
			perror("Failed to create a thread");
		}
	}
	int total = 0;
	for (int i = 0; i < 2; ++i) {
		int* r;
		if (pthread_join(th[i], (void**) &r)) {
			perror("Failed to join a thread");
		}
		total += *r;
		free(r);
	}
	printf("Total sum: %d\n", total);
	return 0;
}
