#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void ParallelFill(double* X, double* Y, const size_t N)
{
    #pragma omp parallel for
    for (size_t i = 0; i < N; i++)
    {
        X[i] = Y[i];
    }
}

void RandomMatrix(double* A, const size_t N, const size_t M)
{
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < M; ++j)
        {
            A[i * M + j] = 1.0 * rand() / RAND_MAX / N / 2;
            if (i == j) {
                A[i * M + j] += 0.5;
            }
        }
    }
}

double sqr(const double x) { return x * x; }

int converge(double* X, double* prev_X, const size_t N, const double eps)
{
	double norm = 0;
    #pragma omp parallel for reduction(+: norm)
	for (size_t i = 0; i < N; i++)
    {
		norm += sqr(X[i] - prev_X[i]);
    }
	return norm < sqr(eps);
}

void seidel(double* A, double* X, double* b, const size_t N, const double eps)
{
    double* prev_X = malloc(N * 1 * sizeof(double));
    do
    {
        ParallelFill(prev_X, X, N);
        for (size_t i = 0; i < N; i++)
        {
            double var = 0;
            #pragma omp parallel reduction(+: var)
            {
                #pragma omp sections nowait
                {
                    #pragma omp section
                    for (size_t j = 0; j < i; j++)
                    {
                        var += (A[i * N + j] * X[j]);
                    }

                    #pragma omp section
                    for (size_t j = i + 1; j < N; j++)
                    {
                        var += (A[i * N + j] * prev_X[j]);
                    }
                }
            }
            X[i] = (b[i] - var) / A[i * N + i];
        }
        /*
        for (size_t i = 0; i < N; ++i) {
            printf("%f ", X[i]);
        }
        printf("\n");
        */
    } while (!converge(X, prev_X, N, eps));

    free(prev_X);
}

int validate_ans(double* A, double* X, double* b, const size_t N, const double eps)
{
    double* b_cur = malloc(N * 1 * sizeof(double));
    for (size_t i = 0; i < N; ++i)
    {
        b_cur[i] = 0;
    }
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; j++) {
            b_cur[i] += A[i * N + j] * X[j];
        }
    }
    int res = converge(b_cur, b, N, eps);
    free(b_cur);
    return res;
}

int main()
{
    const size_t N = 20000;
    const double eps = 1e-7;
    const size_t nThreads = 3;

    double* A = malloc(N * N * sizeof(double));
    double* X = malloc(N * 1 * sizeof(double));
    double* b = malloc(N * 1 * sizeof(double));

    srand(time(0));
    RandomMatrix(A, N, N);
    memset(X, 0, N * 1 * sizeof(X));
    RandomMatrix(b, N, 1);

    omp_set_num_threads(nThreads);
    double start_parallel = omp_get_wtime();
    seidel(A, X, b, N, eps);
    double finish_parallel = omp_get_wtime();
    printf("Time = %lf\n", finish_parallel - start_parallel);

    if (validate_ans(A, X, b, N, eps)) {
        printf("ok\n");
    } else {
        printf("wrong answer\n");
    }

    free(A);
    free(X);
    free(b);
    return 0;
}