#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NUM 2

void* routine(void* args) {
	sleep(1);
	printf("Finished execution\n");
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[THREAD_NUM];
	pthread_attr_t detachedThread;

	pthread_attr_init(&detachedThread);
	pthread_attr_setdetachstate(&detachedThread, PTHREAD_CREATE_DETACHED);

	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_create(th + i, &detachedThread, routine, NULL)) {
			perror("Failed to create thread");
		}
//		pthread_detach(th[i]);
	}
//	for (int i = 0; i < THREAD_NUM; ++i) {
//		if (pthread_join(th[i], NULL)) {
//			perror("Failed to join a thread");
//		}
//	}
	pthread_attr_destroy(&detachedThread);
	pthread_exit(0);
}
