#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/syscall.h>

#define THREAD_NUM 2


void* roll_dice(void* args) {
	pthread_t th = pthread_self();
	printf("%ul\n", th);
	printf("%ul\n", (pid_t) syscall(SYS_gettid));
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];

	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_create(th + i, NULL, roll_dice, NULL)) {
			perror("Failed to create thread");
		}
//		printf("%ul\n", th[i]);
	}
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_join(th[i], NULL)) {
			perror("Failed to join a thread");
		}
	}
	return 0;
}
