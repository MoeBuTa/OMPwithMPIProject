#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "modules/array-gen.c"
#include "IOs/serialIO.c"
#include <mpi.h>
#include "IOs/mpiIO.c"
#include "measurement/mpi-time-measure.c"

struct stat st;

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    //  check the number of arguments
    if (argc != 3)
    {
        if (rank == 0)
        {
            fprintf(stderr, "Usage example: mpiexec -n x %s quick/merge/enum 100000\n", argv[0]);
        }

        exit(EXIT_FAILURE);
    }

    int length = atoi(argv[2]);
    //length of the buffer in each process
    int subBufLen = length / size;
    // length of the array in each process
    int subArrLen = (rank + 1 < size) ? subBufLen : (length - (subBufLen * rank));
    double *subArr;

    // check if file exists
    stat("mpi_unsorted_array.bin", &st);
    if (st.st_size != length * sizeof(double))
    {
        if (rank == 0)
        {
            if (st.st_size != 0)
            {
                printf("Deleting previous unsorted array files...\n");
                int deleteMPI = remove("mpi_unsorted_array.bin");
                int deleteSERIAL = remove("serial_unsorted_array.bin");
                if (!deleteMPI && !deleteSERIAL)
                    printf("Deleted successfully!\n");
            }

            double *a = randomArray(length);
            serialWrite(a, length);
            for (int i = 1; i < size; i++)
            {
                subArr = a + subBufLen * i;
                int arrLen = (i + 1 < size) ? subBufLen : (length - (subBufLen * i));
                MPI_Send(subArr, arrLen, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }

            // set subArr for root process
            double *temp = (double *)malloc(subArrLen * sizeof(double));
            memcpy(temp, a, subArrLen * sizeof(double));
            subArr = temp;
            free(a);
        }
        else
        {
            //recieve subArrLen and subArr from root process
            double *recvdtmp = (double *)malloc(subArrLen * sizeof(double));
            MPI_Recv(recvdtmp, subArrLen, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            subArr = recvdtmp;
        }
        // write array in mpi mode
        mpiWrite(subArr, subArrLen, subBufLen, rank, size, status);
    }

    // compute mpi sort
    if (strcmp(argv[1], "all") == 0)
    {
        mpiSortMeasure("quick", subArrLen, subBufLen, length, rank, size, status);
        mpiSortMeasure("merge", subArrLen, subBufLen, length, rank, size, status);
        mpiSortMeasure("enum", subArrLen, subBufLen, length, rank, size, status);
    }
    else
        mpiSortMeasure(argv[1], subArrLen, subBufLen, length, rank, size, status);

    MPI_Finalize();
}
