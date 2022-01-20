#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 8

typedef struct Task {
	int a, b;
} Task;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

Task taskQueue[256];
int taskCount = 0;

void executeTask(Task* task) {
	usleep(50000);
	int result = task->a + task->b;
	printf("The sum of %d and %d is %d\n", task->a, task->b, result);
}

void submitTask(Task task) {
	pthread_mutex_lock(&mutexQueue);
	taskQueue[taskCount++] = task;
	pthread_mutex_unlock(&mutexQueue);
	pthread_cond_signal(&condQueue);
}

void* startThread(void* args) {
	for (int i = 0; i < 100; ++i) {
		Task task;
		pthread_mutex_lock(&mutexQueue);
		while (taskCount == 0) {
			pthread_cond_wait(&condQueue, &mutexQueue);
		}
		task = taskQueue[0];
		for (int i = 0; i < taskCount - 1; ++i) {
			taskQueue[i] = taskQueue[i + 1];
		}
		--taskCount;
		pthread_mutex_unlock(&mutexQueue);

		executeTask(&task);
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];
	srand(time(NULL));
	pthread_mutex_init(&mutexQueue, NULL);
	pthread_cond_init(&condQueue, NULL);
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_create(th + i, NULL, startThread, NULL)) {
			perror("Failed to create a thread");
		}
	}
	for (int i = 0; i < 200; ++i) {
		Task t = {
				.a = rand() % 100,
				.b = rand() % 100
		};
		submitTask(t);
	}
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	pthread_mutex_destroy(&mutexQueue);
	pthread_cond_destroy(&condQueue);
	return 0;
}
