all:
	mpicc -o mpisort mpisort.c
	mpicc -fopenmp -o ompmpisort ompmpisort.c
	gcc -o serialsort serialsort.c
	gcc -fopenmp -o ompsort ompsort.c