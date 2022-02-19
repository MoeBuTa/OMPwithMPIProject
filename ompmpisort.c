#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "modules/array-gen.c"
#include "IOs/serialIO.c"
#include <mpi.h>
#include "measurement/ompmpi-time-measure.c"

struct stat st;

int main(int argc, char *argv[])
{
    int rank, size, provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    //  check the number of arguments
    if (argc != 4)
    {
        if (rank == 0)
        {
            fprintf(stderr, "Usage example: mpiexec -n x %s quick/merge/enum 100000 y\n", argv[0]);
        }

        exit(EXIT_FAILURE);
    }

    int length = atoi(argv[2]);
    int threads = atoi(argv[3]);
    //length of the buffer in each process
    int subBufLen = length / size;
    // length of the array in each process
    int subArrLen = (rank + 1 < size) ? subBufLen : (length - (subBufLen * rank));
    double *subArr;

    // compute mpi sort
    if (strcmp(argv[1], "all") == 0)
    {
        ompmpiSortMeasure("quick", subArrLen, subBufLen, length, rank, size, threads, status);
        ompmpiSortMeasure("merge", subArrLen, subBufLen, length, rank, size, threads, status);
        ompmpiSortMeasure("enum", subArrLen, subBufLen, length, rank, size, threads, status);
    }
    else
        ompmpiSortMeasure(argv[1], subArrLen, subBufLen, length, rank, size, threads, status);

    MPI_Finalize();
}
