#include <iostream>
#include <vector>
#include <chrono>
#include <random>

constexpr int N = 256;

void matmul(float A[][N], float B[][N], float C[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0f;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
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

    auto start = std::chrono::high_resolution_clock::now();
    matmul(A, B, C);
    auto end = std::chrono::high_resolution_clock::now();
    auto ntime = std::chrono::duration<double, std::milli>(end - start).count();
    
    std::cout << "time " << ntime << " ms\n";
    std::cout << "output [0][0] = " << C[0][0] << " and output [255][255] = " << C[N-1][N-1] << "\n";

    return 0;

}