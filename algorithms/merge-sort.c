/**
 * @brief  merge sort algorithm
 * @note   
 * @param  *a: array to be sorted
 * @param  left: leftmost index
 * @param  mid: mid index
 * @param  right: rightmost index
 * @retval None
 */
void merge(double *a, int left, int mid, int right)
{
    double *temp = (double *)malloc((right - left + 1) * sizeof(double));
    int temp_left = left;
    int temp_mid = mid;
    int i = 0;

    //pop the smaller element each time and push it into temp
    while (temp_left < mid && temp_mid <= right)
    {
        if (a[temp_left] < a[temp_mid])
        {
            temp[i++] = a[temp_left++];
        }
        else
        {
            temp[i++] = a[temp_mid++];
        }
    }

    //Push remaining in the either of the two parts of the array into temp;
    while (temp_left < mid)
    {
        temp[i++] = a[temp_left++];
    }
    while (temp_mid < right)
    {
        temp[i++] = a[temp_mid++];
    }

    //copy temp into the corresponding address of the array
    for (int j = 0; j < i; j++)
    {
        a[left + j] = temp[j];
    }
    free(temp);
}

/**
 * @brief  merge sort algorithm
 * @note   
 * @param  *a: array to be sorted
 * @param  left: leftmost index
 * @param  right: rightmost index
 * @retval None
 */
void mergeSort(double *a, int left, int right)
{
    if (left == right)
    {
        return;
    }

    //divide array into two by mid index
    int mid = (left + right) / 2;

    //traverse the array
    mergeSort(a, left, mid);
    mergeSort(a, mid + 1, right);
    merge(a, left, mid + 1, right);
}
