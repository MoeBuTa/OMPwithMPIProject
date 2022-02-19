#include "quick-sort.c"


/**
 * @brief  parallel solution for quick sort
 * @note   
 * @param  *a: array to be sorted
 * @param  left: start index
 * @param  right: end index
 * @retval None
 */
void parallelQuickSort(double *a, int left, int right)
{

    if (left >= right)
        return;
    int mid = partition(a, left, right);

    //generate task, make task untied, copy initialized mid pointer to every thread
    //executed parallelly or not is depend on the if condition
#pragma omp task firstprivate(mid, left) untied if (mid - left > 10000)
    parallelQuickSort(a, left, mid);
#pragma omp task firstprivate(mid, right) untied if (right - mid - 1 > 10000)
    parallelQuickSort(a, mid + 1, right);
}


/**
 * @brief  call function for parallel quick sort
 * @note   
 * @param  *a: array to be sorted
 * @param  left: start index
 * @param  right: end index
 * @param  threads: number of threads
 * @retval None
 */
void callParallelQuickSort(double *a, int left, int right, int threads)
{
#pragma omp parallel num_threads(threads)
    {
#pragma omp single
        parallelQuickSort(a, left, right);
    }
}