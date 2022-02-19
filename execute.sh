#!/bin/bash

for array_length in 1000 10000 100000 300000
do
    # test number of processes from 2 to 10
    for process_num in 2 3 4 5 6 7 8 9 10
    do
        # do three times for each case
        for iteration in 1 2 3
        do
            mpiexec -n $process_num mpisort all $array_length

            # test number of threads from 2 to 8
            for thread_num in 2 3 4 5 6 7 8
            do
                mpiexec -n $process_num ompmpisort all $array_length $thread_num
            done
        done
    done

    # test serial only and omp only
    for iteration in 1 2 3
    do
        ./serialsort all $array_length
        for thread_num in 2 3 4 5 6 7 8
        do
            ./ompsort all $array_length $thread_num
        done
    done
done

# test bigger size of array for quick and merge only
for array_length in 1000000 10000000 100000000
do
    for process_num in 2 3 4 5 6 7 8 9 10
    do
        for iteration in 1 2 3
        do
            mpiexec -n $process_num mpisort quick $array_length
            mpiexec -n $process_num mpisort merge $array_length
            for thread_num in 2 3 4 5 6 7 8
            do
                mpiexec -n $process_num ompmpisort quick $array_length $thread_num
                mpiexec -n $process_num ompmpisort merge $array_length $thread_num
            done
        done
    done

    for iteration in 1 2 3
    do
        ./serialsort quick $array_length
        ./serialsort merge $array_length
        for thread_num in 2 3 4 5 6 7 8
        do
            ./ompsort quick $array_length $thread_num
            ./ompsort merge $array_length $thread_num
        done
    done
done