#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NUM 8

pthread_barrier_t barrierRolledDice;
pthread_barrier_t barrierStatusSet;

int diceValues[THREAD_NUM];
int status[THREAD_NUM] = { 0 };

void* roll_dice(void* args) {
	int index = *(int*)args;

	for (int i = 1; i <= 10; ++i) {
		diceValues[index] = rand() % 6 + 1;
		pthread_barrier_wait(&barrierRolledDice);
		pthread_barrier_wait(&barrierStatusSet);
		if (status[index] == 1) {
			printf("(%d rolled %d) I won\n", index, diceValues[index]);
		} else {
			printf("(%d rolled %d) I lost\n", index, diceValues[index]);
		}
	}
	free(args);
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];

	pthread_barrier_init(&barrierRolledDice, NULL, THREAD_NUM + 1);
	pthread_barrier_init(&barrierStatusSet, NULL, THREAD_NUM + 1);
	srand(time(NULL));
	for (int i = 0; i < THREAD_NUM; ++i) {
		int* a = malloc(sizeof *a);
		*a = i;
		if (pthread_create(th + i, NULL, roll_dice, a)) {
			perror("Failed to create thread");
		}
	}
	for (int i = 1; i <= 10; ++i) {
		pthread_barrier_wait(&barrierRolledDice);
		int max = 0;
		for (int i = 0; i < THREAD_NUM; ++i) {
			if (diceValues[i] > max) {
				max = diceValues[i];
			}
		}
		for (int i = 0; i < THREAD_NUM; ++i) {
			if (diceValues[i] == max) {
				status[i] = 1;
			} else {
				status[i] = 0;
			}
		}
		sleep(1);
		printf("===%2d ROUND===\n", i);
		pthread_barrier_wait(&barrierStatusSet);
	}
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	pthread_barrier_destroy(&barrierRolledDice);
	pthread_barrier_destroy(&barrierStatusSet);
	return 0;
}
