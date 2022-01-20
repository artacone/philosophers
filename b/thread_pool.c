#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 8

typedef struct Task {
	void (*taskFunction)(int, int);
	int arg1, arg2;
} Task;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

Task taskQueue[256];
int taskCount = 0;

//void sumAndProduct(int a, int b) {
//	usleep(50000);
//	int sum = a + b;
//	int product = a * b;
//	printf("Sum and product of %d and %d is %d and %d\n", a, b, sum, product);
//}

void sum(int a, int b) {
	usleep(50000);
	int sum = a + b;
	printf("Sum of %d and %d is %d\n", a, b, sum);
}

void product(int a, int b) {
	usleep(50000);
	int product = a * b;
	printf("Product of %d and %d is %d\n", a, b, product);
}

void executeTask(Task* task) {
	task->taskFunction(task->arg1, task->arg2);
//	usleep(50000);
//	int result = task->a + task->b;
//	printf("The sum of %d and %d is %d\n", task->a, task->b, result);
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
				.taskFunction = (i % 2 == 0) ? sum : product,
				.arg1 = rand() % 100,
				.arg2 = rand() % 100
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
