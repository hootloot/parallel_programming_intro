#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 512
#define TILE 32

double A[N][N], B[N][N], C[N][N];

int current_num_threads;

typedef struct {
    int id;
} thread_arg_t;

void *multiply(void *arg) {
    int id = ((thread_arg_t *)arg)->id;
    int chunk = N / current_num_threads;
    int row_start = id * chunk;
    int row_end = row_start + chunk;

    for (int i = row_start; i < row_end; i++)
        for (int j = 0; j < N; j++)
            C[i][j] = 0.0;

    for (int ii = row_start; ii < row_end; ii += TILE) {
        for (int jj = 0; jj < N; jj += TILE) {
            for (int kk = 0; kk < N; kk += TILE) {
                for (int i = ii; i < ii + TILE && i < row_end; i++) {
                    for (int j = jj; j < jj + TILE && j < N; j++) {
                        double sum = C[i][j];
                        for (int k = kk; k < kk + TILE && k < N; k++) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] = sum;
                    }
                }
            }
        }
    }
    return NULL;
}

int main() {
    int test_threads[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
    int num_tests = sizeof(test_threads) / sizeof(test_threads[0]);

    srand(time(NULL));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = (double)(rand() % 10);
            B[i][j] = (double)(rand() % 10);
        }

    printf("\n%10s | %15s\n", "Threads", "Time");

    for (int t = 0; t < num_tests; t++) {
        current_num_threads = test_threads[t];

        pthread_t threads[current_num_threads];
        thread_arg_t args[current_num_threads];

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int i = 0; i < current_num_threads; i++) {
            args[i].id = i;
            pthread_create(&threads[i], NULL, multiply, &args[i]);
        }
        for (int i = 0; i < current_num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;

        printf("%10d | %15.4f\n", current_num_threads, elapsed);
    }

    return 0;
}
