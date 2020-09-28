#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>

#include "lib/tester.hpp"
#include "matrix/matrix.h"

Matrix creteRandomMatrix(const size_t n, const size_t m, std::mt19937_64& rnd) {
    Matrix ans(n, m, 0);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            ans[i][j] = rnd() % 10 - 5;
        }
    }
    return ans;
}

int testPerf() {
    using TestFunc = std::function<bool(const size_t, const size_t)>;
    TestFunc test_func = [](
        const size_t n,
        const size_t seed)
        {
            std::mt19937_64 rnd(seed);
            auto a = creteRandomMatrix(n, n, rnd);
            auto b = creteRandomMatrix(n, n, rnd);
            //std::cout << "created" << std::endl;

            auto norm_start = std::chrono::steady_clock::now();
            auto norm_matmul = slowMatMul(a, b);
            auto norm_finish = std::chrono::steady_clock::now();
            auto norm_time = std::chrono::duration_cast<std::chrono::microseconds>(norm_finish - norm_start).count() / 1000;

            int shtrassen_time = 0;
            Matrix shtrassen_matmul = norm_matmul;
            if (n <= 512) {
                auto shtrassen_start = std::chrono::steady_clock::now();
                shtrassen_matmul = a * b;
                auto shtrassen_finish = std::chrono::steady_clock::now();
                shtrassen_time = std::chrono::duration_cast<std::chrono::microseconds>(shtrassen_finish - shtrassen_start).count() / 1000;
            }


            std::cout << "|" << std::setw(4) << n
                << "|" << std::setw(9) << norm_time
                << "|" << std::setw(14) << shtrassen_time
                << "|\n";
            return shtrassen_matmul == norm_matmul;
        };
    Tester<TestFunc> matrix_tester(test_func);

    std::cout.setf(std::ios_base::left);
    std::cout.setf(std::ios_base::fixed);
    std::cout.precision(4);

    std::cout << "+----+---------+--------------+" << std::endl;
    std::cout << "|N   |norm, ms |shtrassen, ms |" << std::endl;
    std::cout << "+----+---------+--------------+" << std::endl;
    for (size_t n = 128; n <= 4096; n *= 2) {
        matrix_tester.execute(n, 42);
        std::cout << "+----+---------+--------------+" << std::endl;
    }

    matrix_tester.printStatistics();
    return matrix_tester.getTotal() != matrix_tester.getSuccess();
}

int main() {
    return testPerf();
}
