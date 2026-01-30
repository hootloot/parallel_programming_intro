#include <iostream>
#include <vector>
#include <chrono>
#include <random>

constexpr int N = 256;

void matmul(float A[][N], float B[][N], float C[][N], int block_size) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            C[i][j] = 0.0f;

    for (int r = 0; r < N; r += block_size) {
        for (int c = 0; c < N; c += block_size) {
            for (int s = 0; s < N; s += block_size) {
                
                for (int i = r; i < r + block_size; i++) {
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
}

int main() {
    static float A[N][N], B[N][N], C[N][N];
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = dist(rng);
            B[i][j] = dist(rng);
        }
    }
    int tests[4] = {4, 16, 32, 128};
    for (int i = 0; i < 4; i++) {
        int curr_size = tests[i];
        auto start = std::chrono::high_resolution_clock::now();
        matmul(A, B, C, curr_size);
        auto end = std::chrono::high_resolution_clock::now();
        auto ntime = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "time " << ntime << " ms" << " at tile size = " << curr_size << "\n";
    }
    return 0;
}