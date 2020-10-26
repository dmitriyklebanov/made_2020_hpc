#include <unistd.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const size_t MAXN = 16;
const size_t MAX_MESSAGE_SIZE = 50;

void generate_message(char* output) {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    srand(time(NULL));
    sleep(rand() % 3 + 1);
    sprintf(
        output,
        "time = [%02d:%02d:%dT%02d:%02d:%02d]",
        timeinfo->tm_mday,
        timeinfo->tm_mon + 1,
        timeinfo->tm_year + 1900,
        timeinfo->tm_hour,
        timeinfo->tm_min,
        timeinfo->tm_sec
    );
}

int generate_next_num(int* nums, const int psize) {
    srand(time(NULL));
    int used[MAXN];
    memset(used, 0, MAXN * sizeof(int));
    for (size_t i = 1; i <= nums[0]; i++) {
        used[nums[i]] = 1;
    }
    int n = rand() % (psize - nums[0]);
    int next = 0;
    while (n || used[next]) {
        if (!used[next]) {
            --n;
        }
        ++next;
    }
    return next;
}

void process_send(int* nums, char* messages, const int prank, const int psize) {
    nums[++nums[0]] = prank;
    generate_message(messages + (nums[0] - 1) * MAX_MESSAGE_SIZE);
    if (nums[0] == psize) {
        printf("id: message\n");
        char to_print[MAX_MESSAGE_SIZE];
        for (size_t i = 1; i <= psize; ++i) {
            strcpy(to_print, messages + (i - 1) * MAX_MESSAGE_SIZE);
            printf("%d: \'%s\'\n", nums[i], to_print);
        }
    } else {
        int next = generate_next_num(nums, psize);
        MPI_Ssend(nums, MAXN, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Ssend(messages, MAXN * MAX_MESSAGE_SIZE, MPI_CHAR, next, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char** argv)
{
    int prank, psize;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);

    int nums[MAXN];
    memset(nums, 0, MAXN * sizeof(int));
    char messages[MAXN * MAX_MESSAGE_SIZE];
    memset(messages, 0, MAXN * MAX_MESSAGE_SIZE * sizeof(char));
    if (prank != 0) {
        MPI_Recv(nums, MAXN, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(messages, MAXN * MAX_MESSAGE_SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    process_send(nums, messages, prank, psize);
    MPI_Finalize();
    return 0;
}
