#include <stdio.h>
#include <sys/time.h>
#include "../IOs/serialIO.c"
#include "../modules/algo-select.c"
#include "../modules/write_data.c"

struct timeval start, end;

/**
 * @brief   measure the time elasped by omp sorting solution
 * @note   
 * @param  algo: algorithm parameter - quick, merge, enumeration
 * @param  length: array length
 * @param threads: number of threads
 * @retval None
 */
void *ompSortMeasure(char *algo, int length, int threads)
{
    double *a = serialRead("mpi_unsorted_array.bin", length);
    printf("Computing OMP %s sorting...\n", algo);
    gettimeofday(&start, NULL);
    selectParallelAlgorithm(algo, a, length, threads);
    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("time spent by OMP %s sort: %lfs\n", algo, delta);
    write_csv(length, algo, 1, threads, delta);
}