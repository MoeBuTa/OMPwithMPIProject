#include <stdio.h>
#include "../algorithms/enumeration-sort.c"
#include "../algorithms/quick-sort-parallel.c"
#include "../algorithms/merge-sort-parallel.c"
#include "../algorithms/enumeration-sort-parallel.c"
/**
 * @brief  select one of three algorithms to perform sorting.
 * @note   
 * @param  algo: algorithm parameter - quick,  merge, enumeration
 * @param  *a: pointer of array to be sorted
 * @param  length: length of the array to be sorted
 * @retval None
 */
void selectAlgorithm(char *algo, double *a, int length)
{
    if (strcmp(algo, "quick") == 0)
        quickSort(a, 0, length - 1);
    else if (strcmp(algo, "merge") == 0)
        mergeSort(a, 0, length - 1);
    else if (strcmp(algo, "enum") == 0)
        enumerationSort(a, length);
}

/**
 * @brief  select one of three algorithms to perform parallel sorting.
 * @note   
 * @param  algo: algorithm parameter - quick,  merge, enumeration
 * @param  *a: pointer of array to be sorted
 * @param  length: length of the array to be sorted
 * @retval None
 */
void selectParallelAlgorithm(char *algo, double *a, int length, int threads)
{
    if (strcmp(algo, "quick") == 0)
        callParallelQuickSort(a, 0, length - 1, threads);
    else if (strcmp(algo, "merge") == 0)
        callParallelMergeSort(a, 0, length - 1, threads);

    else if (strcmp(algo, "enum") == 0)
        parallelEnumerationSort(a, length, threads);
}