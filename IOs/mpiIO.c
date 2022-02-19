#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>
/**
 * @brief  using mpi to read the binary file
 * @note   
 * @param  *filename: the name of the file to be read
 * @param  subArrLen: length of the array in each process
 * @param  rank: rank of the calling process
 * @param  size: number of processes in the communicator
 * @param  status: mpi status
 * @retval portion of the unsorted array read by each process
 */
double *mpiRead(char *filename, int subArrLen, int subBufLen, int rank, int size, MPI_Status status)
{
    MPI_File fh;
    double *buf = (double *)malloc(subArrLen * sizeof(double));
    int offsetByte = rank * subBufLen * sizeof(double);
    MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

    //Combining File Seek & Read in One Step for Thread Safety
    MPI_File_read_at_all(fh, offsetByte, buf, subArrLen, MPI_DOUBLE, &status); // MPI_File_read_at_all(file_handle, offset, buf, num_items, datatype, status)
    MPI_File_close(&fh);                                                       //Closing a File
    return buf;
}

/**
 * @brief  using mpi to write the array into the binary file
 * @note   
 * @param  *tmp: pointer of sub array to be written
 * @param  subArrLen: length of the array in each process
 * @param  subBufLen: buffer of the array in each process
 * @param  rank:  rank of the calling process
 * @param  size: number of processes in the communicator
 * @param  status: mpi status
 * @retval None
 */
void mpiWrite(double *tmp, int subArrLen, int subBufLen, int rank, int size, MPI_Status status)
{
    MPI_File fhw;
    int offsetByte = rank * subBufLen * sizeof(double);
    char *filename = "mpi_unsorted_array.bin";
    if (rank == 0)
    {
        printf("Writing array in MPI mode into \"mpi_unsorted_array.bin\"...\n");
    }

    MPI_File_open(MPI_COMM_WORLD, filename,
                  MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fhw);
    MPI_File_write_at_all(fhw, offsetByte, tmp, subArrLen,
                          MPI_DOUBLE, &status); // MPI_File_write_at_all(file_handle, offset, buf, count, datatype)
    MPI_File_close(&fhw);                       //Closing a File
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("MPI file writing successful!\n");
        printf("--------------------------\n");
    }
}
