#include <stdlib.h>
#include <time.h>

/**
 * @brief  a function to gerenate random array
 * @note   
 * @param  length: array length
 * @retval pointer of the array
 */
double *randomArray(int length)
{
    double *a = (double *)malloc(length * sizeof(double));
    srand((unsigned)time(NULL));
    for (int i = 0; i < length; i++)
    {
        a[i] = (double)(rand() % 10000 + 1) / (rand() % 100 + 1);
    }
    return a;
}
