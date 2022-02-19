#include <stdio.h>
#include <sys/time.h>
#include "../modules/algo-select.c"
#include "../modules/write_data.c"
#include "../modules/mpi-verification.c"
#include <mpi.h>

struct timeval start, end;


/**
 * @brief  measure the time elasped by mpi solution and verify the result
 * @note   
 * @param  algo: algorithm parameter - quick, merge, enumeration
 * @param  subArrLen: length of the array in each process
 * @param  subBufLen: length of the buffer in each process
 * @param  length: length of the array to be sorted
 * @param  rank:  rank of the calling process
 * @param  size: number of processes in the communicator
 * @param  status: mpi status
 * @retval 
 */
void *mpiSortMeasure(char *algo, int subArrLen, int subBufLen, int length, int rank, int size, MPI_Status status)
{
    double *subArr = mpiRead("mpi_unsorted_array.bin", subArrLen, subBufLen, rank, size, status);
    if (rank == 0)
    {
        printf("Computing MPI %s sort...\n", algo);
        gettimeofday(&start, NULL);
    }
    selectAlgorithm(algo, subArr, subArrLen);
    for (int step = 1; step < size; step = 2 * step)
    {
        // send array length and array to nearby process
        if (rank % (2 * step) != 0)
        {
            MPI_Send(&subArrLen, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
            MPI_Send(subArr, subArrLen, MPI_DOUBLE, rank - step, 0, MPI_COMM_WORLD);
            break;
        }

        // receive array length and array from nearby processes
        if (rank + step < size)
        {
            int receivedtmpLen = 0;
            MPI_Recv(&receivedtmpLen, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
            double *receivedtmp = (double *)malloc(receivedtmpLen * sizeof(double));
            MPI_Recv(receivedtmp, receivedtmpLen, MPI_DOUBLE, rank + step, 0, MPI_COMM_WORLD, &status);
            double *a = mergeSubArr(subArr, receivedtmp, subArrLen, receivedtmpLen);
            free(subArr);
            free(receivedtmp);
            subArr = a;
            subArrLen = subArrLen + receivedtmpLen;
        }
    }
    if (rank == 0)
    {
        gettimeofday(&end, NULL);

        //calculate elapsed time
        double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
        printf("time spent by MPI %s sort: %lfs\n", algo, delta);

        //write result into csv file
        write_csv(length, algo, size, 1, delta);

        //compute serial sort for verification
        double *serial_a = serialSort(algo, length);
        mpiSortVerfication(algo, serial_a, subArr, length);
    }
}

