#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const unsigned int N = 100000000;
const int nThreads = 4;
unsigned int seeds[4];

void seedThreads() {
    int my_thread_id;
    unsigned int seed;

    #pragma omp parallel private(seed, my_thread_id)
    {
        my_thread_id = omp_get_thread_num();
        seed = (unsigned) time(NULL);
        seeds[my_thread_id] = (seed & 0xFFFFFFF0) | (my_thread_id + 1);
    }
}

double sqr(const double x) { return x * x; }

double parallel_pi_calc()
{
    unsigned int sum = 0;
    unsigned int seed, tid, i;

    #pragma omp parallel reduction(+: sum) \
        private(seed, tid, i) \
        shared(seeds)
    {
        tid = omp_get_thread_num();
        seed = seeds[tid];
    	srand(seed);

        #pragma omp parallel for
            for (i = 0; i < N; ++i)
            {
                double r = sqr(((double) rand_r(&seed)) / RAND_MAX) +
                sqr(((double) rand_r(&seed)) / RAND_MAX);
                sum += r <= 1;
            }
    }

    return ((double)sum) / N / nThreads * 4;
}

double sync_pi_calc()
{
    unsigned int sum = 0;
    unsigned int seed, i;
    seed = time(0);
    srand(seed);

    for (i = 0; i < N * nThreads; ++i)
    {
        double r = sqr(((double) rand_r(&seed)) / RAND_MAX) +
        sqr(((double) rand_r(&seed)) / RAND_MAX);
        sum += r <= 1;
    }

    return ((double)sum) / N / nThreads * 4;
}

int main (int argc, char *argv[])
{
    omp_set_num_threads(1);
    double start_sync = omp_get_wtime();
    printf("Sync Pi = %lf\n", sync_pi_calc(N));
    double finish_sync = omp_get_wtime();
    printf("Sync Time = %lf\n", finish_sync - start_sync);


    omp_set_num_threads(nThreads);
    seedThreads();

    omp_set_num_threads(nThreads);
    double start_parallel = omp_get_wtime();
    printf("Parallel Pi = %lf\n", parallel_pi_calc(N));
    double finish_parallel = omp_get_wtime();
    printf("Parallel Time = %lf\n", finish_parallel - start_parallel);

    return 0;
}