#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define NUM_ROUNDS 100

int shared_var = 0;
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *ping_pong(void *arg) {
	long tid = (long)arg;

	while (true) {
		pthread_mutex_lock(&mutex);

		while (count < NUM_ROUNDS) {
			if (shared_var == 0) {
				printf("ping thread %ld\n", tid);
				shared_var = 1;
				count++;
				pthread_cond_broadcast(&cond);
				pthread_cond_wait(&cond, &mutex);
				break;
			} else if (shared_var == 1) {
				printf("pong thread %ld \n", tid);
				shared_var = 0;
				count++;
				pthread_cond_broadcast(&cond);
				pthread_cond_wait(&cond, &mutex);
				break;
			}
			pthread_cond_wait(&cond, &mutex);
			// wait
		}

		if (count >= NUM_ROUNDS) {
			pthread_mutex_unlock(&mutex);
			break;
		}

		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);
	}

	pthread_cond_broadcast(&cond);
	return NULL;
}

int main(void) {
	pthread_t threads[NUM_THREADS];
	for (long i = 0; i < NUM_THREADS; i++) {
		if (pthread_create(&threads[i], NULL, ping_pong, (void *)i) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
}
