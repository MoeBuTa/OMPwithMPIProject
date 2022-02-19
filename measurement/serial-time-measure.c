#include <stdio.h>
#include <sys/time.h>
#include "../IOs/serialIO.c"
#include "../modules/algo-select.c"
#include "../modules/write_data.c"

struct timeval start, end;

/**
 * @brief   measure the time elasped by serial sorting solution
 * @note   
 * @param  algo: algorithm parameter - quick, merge, enumeration
 * @param  length: array length
 * @retval None
 */
void *serialSortMeasure(char *algo, int length)
{
    double *a = serialRead("mpi_unsorted_array.bin", length);
    printf("Computing SERIAL %s sorting...\n", algo);
    gettimeofday(&start, NULL);
    selectAlgorithm(algo, a, length);
    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("time spent by SERIAL %s sort: %lfs\n", algo, delta);
    write_csv(length, algo, 1, 1, delta);
}