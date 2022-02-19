<h1 align=center>CITS5507 High Performance Computing - Project1</h1>

<h2 align=center>Wenxiao Zhang (22792191)</h2>

## Introduction
<p style='text-align: justify;'>

This report contains five sections in total.  
Firstly, in <b>Serial sort pseudocode</b> and <b>Parallel sort pseudocode</b> section, pseudocode are applied to explain each serial sorting algorithm and each parallel sorting algorithm respectively. In <b>Experimental environment</b> section, the software and hardware environment to perform the experiment are introduced. In addtion, <b>Compile method description</b> section mainly describes how the program is compiled. Finally, in <b>Speed analysis</b> section, the results of the experiment are listed and analysed.
</p>

## Sections

- [Introduction](#introduction)
- [Sections](#sections)
- [Serial sort pseudocode](#serial-sort-pseudocode)
  - [1. Serial quick sort Pseudocode](#1-serial-quick-sort-pseudocode)
  - [2. Serial merge sort pseudocode](#2-serial-merge-sort-pseudocode)
  - [3. Serial enumeration sort pseudocode](#3-serial-enumeration-sort-pseudocode)
  - [Summary](#summary)
- [omp sort pseudocode](#omp-sort-pseudocode)
  - [1. omp quick sort pseudocode](#1-omp-quick-sort-pseudocode)
  - [2. omp merge sort pseudocode](#2-omp-merge-sort-pseudocode)
  - [3. omp enumeration sort pseudocode](#3-omp-enumeration-sort-pseudocode)
  - [Summary](#summary-1)
- [Experimental environment](#experimental-environment)
- [Compile method description](#compile-method-description)
- [Speedup analysis](#speedup-analysis)

<br>



<div style="page-break-after: always;"></div>

## Serial sort pseudocode

### 1. Serial quick sort Pseudocode

```c
partition(array, left pointer, right pointer) {
    Initialize pivot <- left pointer;
    while (left pointer less than right pointer) 

        //Move right pointer to left until the while condition is
        // not satisfied.    
        while((array[right pointer] greater than or equal to pivot) and 
            (right pointer not equal to left pointer)) 
            increment right pointer by 1;
        endwhile  

        //Move left pointer to right until the while condition is
        // not satisfied.
        array[left pointer] <- array[right pointer]; 
        while((array[left pointer] less than pivot) and 
            (left pointer not equal to right pointer)) 
            increment left pointer by 1;
        endwhile  
        array[right pointer] <- a[left pointer];
    endwhile
    array[left pointer] <- pivot;
    return left pointer;
}

quickSort(array, left pointer, right pointer) {
    if (left pointer greater than or equal to right pointer)
        return;
    endif
    Initialize mid pointer <- partition(array, left pointer, right pointer);  
    quickSort(array, left pointer, mid pointer);
    quickSort(array, mid pointer + 1, right pointer);
}
```


<div style="page-break-after: always;"></div>

### 2. Serial merge sort pseudocode

```c
merge(array, left index, mid index, right index) {
    Intialize temp <- a temporary memory sizes 
        (right index - left index + 1)*sizeof(double);
    Intialize left pointer <- left index;
    Intialize mid pointer <- mid index;

    //pop the smaller element each time and push it into temp
    while (left pointer less than mid index and 
        mid pointer less than or equal to right index )
        if (array[left pointer] less than array[mid pointer])
            Push a[left pointer] into temp;
            move left pointer to right by 1;
        else
            Push a[mid pointer] into temp;
            move mid pointer to right by 1;
        endif
    endwhile
    
    //Push remaining in the either of the two parts of the array into temp
    while(left pointer less than mid index){
        push a[left pointer] into temp;
        move left pointer to right by 1;
    }
    while(mid pointer less than right index){
        Push a[mid pointer] into temp;
        move mid pointer to right by 1;
    }

    //copy temp into the corresponding address of the array
    array[left index, right index] <- temp
    free temp;
}

mergeSort(array, left index, right index) {
    if (left index equal to right index )
        return;
    endif
    mid index = (left index + right index) / 2;
    mergeSort(array, left index, mid index);
    mergeSort(array, mid index + 1, right index);
    merge(array, left index, mid index + 1, right index);
}
```

<div style="page-break-after: always;"></div>

### 3. Serial enumeration sort pseudocode

```c
enumerationSort(array, length of the array){
    Initialize rank<-0;
    Initialize temp <- a temporary memory sizes length*sizeof(double);
    for i in range (0, length of the array -1)
        reset rank to 0;
        //count the rank of the current element
        for j in range (0, length of the array -1)
            if ((array[i] not equal to array[j]) or 
                (i greater than j and a[i] equal to a[j]))
                increment rank by 1;
            endif
        endfor
    endfor
    copy temp into the array;
    free temp;
}
```

<br>
<br>

### Summary
The average case run time of quick sort is O(nlogn) . This case happens when we dont exactly get evenly balanced partitions. However, it is not a stable algorithm because the swapping of elements is done according to pivot’s position, which means the worst case runtime of quick sort can be O(n^2).

Merge sort is an efficient, stable sorting algorithm with an average, best-case, and worst-case time complexity of O(nlogn). However, it takes more memory resources as it needs to allocate a new set of memory location each time when performing merge operation.

As enumeration sort has a nested for-loop, the time complextiy of which is always O(n^2), it may have the worst performance among these three algorithms.

<div style="page-break-after: always;"></div>

## omp sort pseudocode

### 1. omp quick sort pseudocode
```c

parallelQuickSort(array, left pointer, right pointer){
    if (left pointer greater than or equal to right pointer)
        return;
    endif
    Initialize mid pointer <- partition(array, left pointer, right pointer);  
    #task directive //generate task
        #untied clause //make task untied
            #firstprivate(mid,left) //copy initialized mid, left pointer to every thread
                #if((mid pointer - left pointer) greater than 10000)
                {// executed parallelly or not is depend on the if condition 
                    parallelQuickSort(array, left pointer, mid pointer);
                }
    #task directive //generate task
        #untied clause //make task untied
            #firstprivate(mid, right) //copy initialized mid, right pointer to every thread
                #if((right pointer-mid pointer-1) greater than 10000)
                {// executed parallelly or not is depend on the if condition 
                    parallelQuickSort(array, mid pointer + 1, right pointer);
                }
}


#parallel directive{
    #omp single driective{  //generate tasks
        parallelQuickSort(array, left pointer, right pointer);
    }    
}


```

<div style="page-break-after: always;"></div>

### 2. omp merge sort pseudocode
```c



parallelMergeSort(array, left index, right index) {
    if (left index equal to right index )
        return;
    endif
    #task directive //generate task
        #untied clause //make task untied
            #share(array) //share the array to every thread    
            if ((mid index - left index) greater than 10000)
            {// executed parallelly or not is depend on the if condition 
                mergeSort(array, left index, mid index);
            } 
    
    #task directive //generate task
        #untied clause //make task untied
            #share(array) //share the array to every thread       
                #if((right index-mid index-1) greater than 10000) 
                { // executed parallelly or not is depend on the if condition 
                    mergeSort(array, mid index + 1, right index);
                }
    #taskwait directive{ //execute merge after division in each recursion
            merge(array, left index, mid index + 1, right index);
    }

}

#parallel directive{
    #omp single driective{  //generate tasks
        parallelMergeSort(array, left index, right index);
    }    
}


```

<div style="page-break-after: always;"></div>

### 3. omp enumeration sort pseudocode
```c
enumerationSort(array, length of the array){
    Initialize rank <- 0;
    Initialize temp <- a temporary memory sizes length*sizeof(double);
    #parallel directive{
        //dynamically distribute iterations to each thread
        #omp for dynamic{
            set rank private to every thread{
                for i in range (0, length of the array -1)
                    reset rank to 0;
                    for j in range (0, length of the array -1)
                        if ((array[i] not equal to array[j]) or 
                            (i greater than j and a[i] equal to a[j]))
                            increment rank by 1;
                        endif
                    endfor
                endfor
            }
        //dynamically distribute iterations to each thread
        #omp for dynamic{
            copy temp into the array;
        }
    }

    free temp;
}

```
<br>
<br>

### Summary

When using OpenMP to perform parallel solution, applying task directive to quick sort and merge sort can deal with the rescursive process effectively. As for the enumeration sort, omp for and omp schedule can be applied to this sorting algorithm to dynamically distribute iterations to each thread.

parallel quick sort needs to set the mid pointer (which obtained after partition) to be firstprivate. So that each thread can have a local copy of the mid value, which can increase their performing speed and avoid race condition. 

As for parallel merge sort, each thread may perform the sorting by traversing different parts of the array, so that we can make the array be shared to increase their performing speed. However, as merge sort needs to divide array up before sorting,  it needs a barrier(i.e. taskwait) to achieve this.


<div style="page-break-after: always;"></div>




```c




```

<div style="page-break-after: always;"></div>

## Experimental environment
The experiment is performed in the virtual machine running by VirtualBox. Using Vagrant to set parameters and log in to the virtual machine. Here are the details of the experimental environment:

- **VM Memory**: 4096 MB
- **Number of VM Processor**: 4
- **gcc version**: 7.5.0
- **Operating System**: bento/ubuntu-18.04 powered by VirtualBox
- **Processor**: Intel(R) Core(TM) i5-8250U CPU @ 1.60GHz

<br>

## Compile method description


Source code compile command: *make -f makefile*.

This compiles four programs: **mpisort.c**, **ompmpisort.c**, **serialsort.c**, **ompsort.c**.
  - **mpisort.c** is designed for random array generation, MPI IO, serial IO and verification, and sorting array in MPI mode.
  - **ompmpisort.c** is designed for sorting array in OMP+MPI mode. The unsorted array is read from the file written by MPI IO module of mpisort.c("mpi_unsorted_array.bin").
  - **serialsort.c** is designed for measuring the elapsed time of sorting array in serial mode. The unsorted array is also read from "mpi_unsorted_array.bin".
  - **ompsort.c** is designed for measuring the elapsed time of sorting array in openmp mode. The unsorted array is also read from "mpi_unsorted_array.bin".

Executing **mpisort**: *mpiexec -n **x** mpisort **algo** **length*** 

Executing **ompmpisort**: *mpiexec -n **x** ompmpisort **algo** **length** **y*** 

Executing **serialsort**: *./serialsort **algo** **length***

Executing **ompsort**: *./ompsort **algo** **length** **y***

*x - number of processes(2/3/4/5...), algo - algorithm parameter(quick/merge/enum/all), length - length of the array to be sorted(1000/10000/200000...), y - number of threads(2/3/4/5...)*

Alternatively, you can try to execute the shell script ***./execute.sh***, which will execute all programs for mulitiple times with several number of processes and several number of threads, but it will take a **very long** time. The executing result can be found in **test_data.csv**.

**Note**: 
1. Executing **mpisort** first before executing the rest programs, as only **mpisort** has the random array generation and file written module, and the other programs need to read the file generated by mpisort in order to get the unsorted array.
2. After executing **mpisort**, the **length** parameter must be the same value when excuting the rest programs.

Here is an example of compiling and executing the source code:



![image](1.png)
<br>


## Speedup analysis

 We'll analyse the difference of performance between serial sort and parallel sort by changing the size (50k, 100k, 300k, 1m, 10m, 20m) of the array to be sorted. Applying a 2x3 table, two rows represents the solution type: serial/parallel, three columns represents three algorithms: quick, merge, enumeration. The value in the cells are the test results of the elapsed time in seconds.

<br>






<p style='text-align: justify;'>
We can see quick sort is the fastest sorting algorithm among these three sorting algorithms, followed by merge sort. As enumeration sort has the time complexity of O(n^2), it is much slower than other sorting algorithms, especially when it comes to larger size of array.
<br>
For quick sort, the parallel solution has a better performance with larger size of array. The speedup outcomes of parallel solution varies between 40%~65% faster than its serial solution based on the array size, which means the array size bigger, the speedup of parallel solution more obvious.
<br>
For merge sort, the parallel solution is about 50%~55% faster than its serial solution. Although merge sort has a more stable speedup outcomes of parallel solution, it is slower than quick sort overall.
<br>
For enumeration sort, the parallel solution is about 3 times faster than its serial solution. However, due to the issue of high time complexity, the overall performance of enumeration sort is much poorer than the other two sorting algorithms.
</p>