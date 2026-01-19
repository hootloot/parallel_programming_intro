#include <iostream>
#include <vector>
#include <chrono>
#include <random>

constexpr int N = 256;

void matmul(const float* A, const float* B, float *C) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

int main() {
    std::vector<float> A(N * N), B(N * N), C(N * N);

    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (int i = 0; i < N * N; ++i) {
        A[i] = dist(rng);
        B[i] = dist(rng);
    }

    auto start = std::chrono::high_resolution_clock::now();
    matmul(A.data(), B.data(), C.data());
    auto end = std::chrono::high_resolution_clock::now();
    auto ntime = std::chrono::duration<double, std::milli>(end - start).count();
    
    std::cout << "time " << ntime << " ms\n";
    std::cout << "output [0][0] = " << C[0] << " and output [255][255] = " << C[N*N-1] << "\n";

    return 0;

}