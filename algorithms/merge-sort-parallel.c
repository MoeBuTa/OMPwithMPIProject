#include "merge-sort.c"

/**
 * @brief  parallel solution for merge sort
 * @note   
 * @param  *a: array to be sorted
 * @param  left: start index
 * @param  right: end index
 * @retval None
 */
void parallelMergeSort(double *a, int left, int right)
{
    if (left == right)
    {
        return;
    }

    //divide array into two by mid index
    int mid = (left + right) / 2;

    //generate task, make task untied, share the array to every thread
    //executed parallelly or not is depend on the if condition
#pragma omp task shared(a) firstprivate(left, mid) untied if (mid - left > 10000)
    parallelMergeSort(a, left, mid);
#pragma omp task shared(a) firstprivate(right, mid) untied if (right - mid - 1 > 10000)
    parallelMergeSort(a, mid + 1, right);
#pragma omp taskwait
    merge(a, left, mid + 1, right);
}


/**
 * @brief  call function for parallel merge sort
 * @note   
 * @param  *a: array to be sorted
 * @param  left: start index
 * @param  right: end index
 * @param  threads: number of threads
 * @retval None
 */
void callParallelMergeSort(double *a, int left, int right, int threads)
{
#pragma omp parallel num_threads(threads)
    {
#pragma omp single
        parallelMergeSort(a, left, right);
    }
}