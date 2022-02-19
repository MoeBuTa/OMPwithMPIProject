#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief  a function serially reading the binary file
 * @note   
 * @param  *filename: the name of the file to be read
 * @param  length: array length
 * @retval pointer of the array 
 */
double *serialRead(char *filename, int length)
{
    double *a = (double *)malloc(length * sizeof(double));
    FILE *fp = fopen(filename, "rb");
    fread(a, sizeof(double), length, fp);
    fclose(fp);
    return a;
}

/**
 * @brief  a function verifying the saved file
 * @note   
 * @param  *a: pointer of the array
 * @param  *filename: the name of the file to be verified
 * @param  length: array length
 * @retval true/false
 */
bool serialVerification(double *a, char *filename, int length)
{
    double *b = serialRead(filename, length);
    printf("Verifying accuracy of SERIAL file wirting...\n");
    for (int i = 0; i < length; i++)
    {
        if (a[i] != b[i])
        {
            free(b);
            return false;
        }
    }
    free(b);
    return true;
}

/**
 * @brief  a function serially saving array to a binary file
 * @note   
 * @param  *a: pointer of the array
 * @param  length: array length
 * @retval None
 */
void serialWrite(double *a, int length)
{
    char *filename = "serial_unsorted_array.bin";
    FILE *fp = fopen(filename, "wb+");
    printf("Writing array in SERIAL mode into \"serial_unsorted_array.bin\"...\n");

    fwrite(a, sizeof(double), length, fp);
    fclose(fp);
    if (!serialVerification(a, filename, length))
    {
        printf("An error occurred in writing file in SERIAL mode!\n");
    }
    else
    {
        printf("SERIAL file writing successful!\n");
        printf("--------------------------\n");
    }
}
