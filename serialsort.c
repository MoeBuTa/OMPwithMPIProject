#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "measurement/serial-time-measure.c"

int main(int argc, char *argv[])
{

    //  check the number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage example: %s quick 100000\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int length = atoi(argv[2]);
    if (strcmp(argv[1], "all") == 0)
    {
        serialSortMeasure("quick", length);
        serialSortMeasure("merge", length);
        serialSortMeasure("enum", length);
    }
    else
    {
        serialSortMeasure(argv[1], length);
    }
}
