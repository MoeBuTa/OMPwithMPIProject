#include <stdio.h>

/**
 * @brief  write test data into csv file
 * @note   
 * @param  length: length of the array
 * @param  *algo: algorithm parameter
 * @param  size: number of processes
 * @param  threads: number of threads
 * @param  delta: time elapsed
 * @retval None
 */
void write_csv(int length, char *algo, int size, int threads, double delta)
{
    FILE *fp = fopen("test_data.csv", "a");
    int fileSize = ftell(fp);
    if (fileSize == 0)
    {
        fprintf(fp, "%s,%s,%s,%s,%s\n", "array_length", "algorithm", "process_num", "thread_num", "time_elapsed");
    }
    fprintf(fp, "%i,%s,%i,%i, %lf\n", length, algo, size,threads, delta);
    fclose(fp);
    printf("The result has been recorded into \"test_data.csv\"! \n");
}