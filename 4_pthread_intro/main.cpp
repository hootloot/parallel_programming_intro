#include <iostream>
#include <chrono>
#include <random>
#include <pthread.h>

constexpr int N = 512;
constexpr int NUM_THREADS = 8;

static float A[N][N], B[N][N], C[N][N];

struct ThreadData {
    int start_row;
    int end_row;
    int block_size;
};

void* matmul_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int block_size = data->block_size;
    
    for (int r = data->start_row; r < data->end_row; r += block_size) {
        for (int c = 0; c < N; c += block_size) {
            for (int s = 0; s < N; s += block_size) {
                for (int i = r; i < r + block_size && i < data->end_row; i++) {
                    for (int j = c; j < c + block_size; j++) {
                        float sum = C[i][j];
                        for (int k = s; k < s + block_size; k++) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] = sum;
                    }
                }
            }
        }
    }
    return nullptr;
}

void matmul_parallel(int block_size) {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            C[i][j] = 0.0f;
    
    int rows_per_thread = N / NUM_THREADS;
    
    for (int t = 0; t < NUM_THREADS; t++) {
        thread_data[t].start_row = t * rows_per_thread;
        thread_data[t].end_row = (t == NUM_THREADS - 1) ? N : (t + 1) * rows_per_thread;
        thread_data[t].block_size = block_size;
        pthread_create(&threads[t], nullptr, matmul_thread, &thread_data[t]);
    }
    
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], nullptr);
    }
}

int main() {
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            A[i][j] = dist(rng);
            B[i][j] = dist(rng);
        }
    
    int block_sizes[] = {16, 32, 64, 128};
    
    std::cout << "Matrix: " << N << "x" << N << ", Threads: " << NUM_THREADS << "\n\n";
    
    for (int bs : block_sizes) {
        auto start = std::chrono::high_resolution_clock::now();
        matmul_parallel(bs);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Block size " << bs << ": " << ms << " ms\n";
    }
    
    return 0;
}