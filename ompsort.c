#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "measurement/omp-time-measure.c"

int main(int argc, char *argv[])
{

    //  check the number of arguments
    if (argc != 4)
    {
        fprintf(stderr, "Usage example: %s quick 100000 y\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int length = atoi(argv[2]);
    int threads = atoi(argv[3]);
    if (strcmp(argv[1], "all") == 0)
    {
        ompSortMeasure("quick", length, threads);
        ompSortMeasure("merge", length, threads);
        ompSortMeasure("enum", length, threads);
    }
    else
    {
        ompSortMeasure(argv[1], length, threads);
    }
}
