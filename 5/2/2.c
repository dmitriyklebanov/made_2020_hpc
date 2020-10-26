#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const size_t MAXN = 100 + 2;
const size_t N_ITER = 1000;

const size_t BOUNDARY_STATEMENT = 2;
const size_t RULE = 30;
const size_t DO_PRINT = 0;

void sync(int* nums, const int prank, const int psize) {
    MPI_Status status;
    if (prank % 2) {
        if (prank != 0) {
            MPI_Recv(&nums[0], 1, MPI_INT, prank - 1, 0, MPI_COMM_WORLD, &status);
            MPI_Ssend(&nums[1], 1, MPI_INT, prank - 1, 0, MPI_COMM_WORLD);
        }

        if (prank != psize - 1) {
            MPI_Recv(&nums[MAXN - 1], 1, MPI_INT, prank + 1, 0, MPI_COMM_WORLD, &status);
            MPI_Ssend(&nums[MAXN - 2], 1, MPI_INT, prank + 1, 0, MPI_COMM_WORLD);
        }
    } else {
        if (prank != psize - 1) {
            MPI_Ssend(&nums[MAXN - 2], 1, MPI_INT, prank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&nums[MAXN - 1], 1, MPI_INT, prank + 1, 0, MPI_COMM_WORLD, &status);
        }

        if (prank != 0) {
            MPI_Ssend(&nums[1], 1, MPI_INT, prank - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&nums[0], 1, MPI_INT, prank - 1, 0, MPI_COMM_WORLD, &status);
        }
    }

    if (BOUNDARY_STATEMENT == 2) {
        if (prank == 0) {
            MPI_Recv(&nums[0], 1, MPI_INT, psize - 1, 0, MPI_COMM_WORLD, &status);
            MPI_Ssend(&nums[1], 1, MPI_INT, psize - 1, 0, MPI_COMM_WORLD);
        }
        if (prank == psize - 1) {
            MPI_Ssend(&nums[MAXN - 2], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&nums[MAXN - 1], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
    } else {
        if (prank == 0 || prank == psize - 1) {
            nums[0] = nums[1] = BOUNDARY_STATEMENT;
        }
    }
}

void init_nums(int* nums, const int prank, const int psize) {
    srand(time(NULL) * prank);
    for (size_t i = 1; i < MAXN - 1; ++i) {
        nums[i] = rand() % 2;
        // nums[i] = (i == MAXN - 2) && (prank == psize - 1);
    }
}

void sync_print(int* nums, const int prank, const int psize) {
    if (prank == 0) {
        int to_print[(MAXN - 2) * psize];
        for (size_t i = 0; i < MAXN - 2; ++i) {
            to_print[i] = nums[i + 1];
        }
        MPI_Status status;
        for (size_t i = 1; i < psize; ++i) {
            MPI_Recv(to_print + (MAXN - 2) * i, MAXN - 2, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
        for (size_t i = 0; i < (MAXN - 2) * psize; ++i) {
            printf("%c", to_print[i] ? '#' : ' ');
        }
        printf("\n");
    } else {
        MPI_Ssend(nums + 1, MAXN - 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}

void process(int* nums, int* tmp) {
    for (size_t i = 1; i < MAXN - 1; ++i) {
        tmp[i] = (RULE & (1 << ((nums[i - 1] << 2) + (nums[i] << 1) + (nums[i + 1])))) > 0;
    }
    for (size_t i = 1; i < MAXN - 1; ++i) {
        nums[i] = tmp[i];
    }
}

int main(int argc, char** argv)
{
    int prank, psize;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);

    double time_elapsed = MPI_Wtime();

    int nums[MAXN], tmp[MAXN];
    memset(nums, 0, MAXN * sizeof(int));
    memset(tmp, 0, MAXN * sizeof(int));
    init_nums(nums, prank, psize);
    for (size_t i = 0; i < N_ITER; ++i) {
        sync(nums, prank, psize);
        if (DO_PRINT) {
            sync_print(nums, prank, psize);
        }
        process(nums, tmp);
    }

    if (!DO_PRINT) {
        sync_print(nums, prank, psize);
    }

    if (prank == 0) {
        time_elapsed = MPI_Wtime() - time_elapsed;
        printf("Size = %d, Num iters = %d, Time = %lf\n", psize * (MAXN - 2), N_ITER, time_elapsed);
    }

    MPI_Finalize();
    return 0;
}
