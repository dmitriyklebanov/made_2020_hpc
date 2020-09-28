#include <functional>
#include <random>

#include "lib/tester.hpp"
#include "matrix/matrix.h"

Matrix creteRandomMatrix(const size_t n, const size_t m, std::mt19937_64& rnd) {
    Matrix ans(n, m, 0);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            ans[i][j] = rnd() % 100 - 50;
        }
    }
    return ans;
}

int testMatMul() {
    using TestFunc = std::function<bool(const size_t, const size_t, const size_t,
        const size_t, const size_t)>;
    TestFunc test_func = [](
        const size_t sz1_n,
        const size_t sz1_m,
        const size_t sz2_n,
        const size_t sz2_m,
        const size_t seed)
        {
            std::mt19937_64 rnd(seed);
            auto a = creteRandomMatrix(sz1_n, sz1_m, rnd);
            auto b = creteRandomMatrix(sz2_n, sz2_m, rnd);
            auto fast_matmul = a * b;
            auto slow_matmul = slowMatMul(a, b);
            return fast_matmul == slow_matmul;
        };
    Tester<TestFunc> matrix_tester(test_func);

    //matrix_tester.execute(1, 2, 2, 2, 42);
    matrix_tester.execute(2, 3, 3, 4, 42);
    matrix_tester.execute(7, 16, 16, 8, 42);
    matrix_tester.execute(7, 16, 16, 8, 43);
    matrix_tester.execute(7, 16, 16, 8, 44);
    matrix_tester.execute(7, 16, 16, 8, 45);

    matrix_tester.printStatistics();
    return matrix_tester.getTotal() != matrix_tester.getSuccess();
}

int main() {
    return testMatMul();
}
