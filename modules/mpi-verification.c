#include <stdio.h>

/**
 * @brief  verify the solution of mpi sort
 * @note   
 * @param  algo: algorithm parameter - quick,  merge, enumeration
 * @param  *serial_a: result sorted by serial solution
 * @param  *mpi_a: result sorted by mpi solution
 * @param  length: length of the array
 * @retval None
 */
void mpiSortVerfication(char *algo, double *serial_a, double *mpi_a, int length)
{
    int j = 0;
    printf("Verifying accuracy of MPI %s sort...\n", algo);

    for (int i = 0; i < length; i++)
    {
        j = (j + 1 == length) ? (i + 1) : i;
        if (serial_a[i] != mpi_a[i] || mpi_a[i] > mpi_a[j])
        {
            printf("%lf %lf\n", serial_a[i], mpi_a[i]);
            printf("An error occurred in MPI %s sort!\n", algo);
            return;
        }
    }
    printf("MPI %s sort successful!\n", algo);
    printf("--------------------------\n");
}

/**
 * @brief  merge two sorted arrays
 * @note   
 * @param  *a: pointer of array a to be merged
 * @param  *b: pointer of array b to be merged (received from another process)
 * @param  lengthA: length of array a
 * @param  lengthB: length of array b
 * @retval merged array
 */
double *mergeSubArr(double *a, double *b, int lengthA, int lengthB)
{
    double *result = (double *)malloc((lengthA + lengthB) * sizeof(double));
    int i = 0;
    int j = 0;

    for (int k = 0; k < lengthA + lengthB; k++)
    {
        if (i >= lengthA)
        {
            result[k] = b[j];
            j++;
        }
        else if (j >= lengthB)
        {
            result[k] = a[i];
            i++;
        }

        // Indices in bounds as i < n1
        // && j < n2
        else if (a[i] < b[j])
        {
            result[k] = a[i];
            i++;
        }

        // v2[j] <= v1[i]
        else
        {
            result[k] = b[j];
            j++;
        }
    }
    return result;
}

/**
 * @brief  compute serial sort for verification
 * @note   
 * @param  algo: algorithm parameter - quick, merge, enumeration
 * @param  length: array length
 * @retval sorted array
 */
double *serialSort(char *algo, int length)
{
    double *a = serialRead("mpi_unsorted_array.bin", length);
    printf("Computing SERIAL %s sort...\n", algo);
    selectAlgorithm(algo, a, length);
    return a;
}