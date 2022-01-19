#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void* roll_dice() {
	srand(time(NULL));
	int *result = malloc(sizeof *result);
	*result = (rand() % 6) + 1;
	return (void*) result;
}

int main(int argc, char* argv[]) {
	int* results[8];
	pthread_t th[8];

	for (int i = 0; i < 8; ++i) {
		if (pthread_create(th + i, NULL, roll_dice, NULL)) {
			return 1;
		}
	}
	for (int i = 0; i < 8; ++i) {
		if (pthread_join(th[i], (void**) results + i)) {
			return 2;
		}
	}
	for (int i = 0; i < 8; ++i) {
		printf("Roll %d: %d\n", i, *(results[i]));
	}
	for (int i = 0; i < 8; ++i) {
		free(results[i]);
	}
	return 0;
}
