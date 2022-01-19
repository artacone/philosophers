#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_barrier_t barrier;


void* routine() {
	printf("Waiting at the barrier...\n");
	sleep(1);
	pthread_barrier_wait(&barrier);
	printf("We passed the barrier\n");
	sleep(1);
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[9];

	pthread_barrier_init(&barrier, NULL, 9);
	for (int i = 0; i < 9; ++i) {
		if (pthread_create(th + i, NULL, routine, NULL)) {
			return 1;
		}
	}
	for (int i = 0; i < 9; ++i) {
		if (pthread_join(th[i], NULL)) {
			return 2;
		}
	}
	pthread_barrier_destroy(&barrier);
	return 0;
}
