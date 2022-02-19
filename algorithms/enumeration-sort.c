/**
 * @brief  enumeration sort algorithm
 * @note   
 * @param  *a: array to be sorted
 * @param  length: length of the array
 * @retval None
 */
void enumerationSort(double *a, int length)
{
    int rank = 0;
    double *temp = (double *)malloc(length * sizeof(double));
    for (int i = 0; i < length; i++)
    {
        rank = 0;
        for (int j = 0; j < length; j++)
        {
            //1. a[i] > a[j] element smaller
            //2. a[i] == a[j] && i > j element which has duplicate value
            if ((a[i] > a[j]) || (i > j && a[i] == a[j]))
            {
                rank++;
            }
        }
        temp[rank] = a[i];
    }

    for (int k = 0; k < length; k++)
    {
        a[k] = temp[k];
    }

    free(temp);
}
