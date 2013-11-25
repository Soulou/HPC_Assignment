all: laplace_mpi

laplace_mpi: laplace_mpi.c matrix_norms.c matrix_utils.c output.c
	mpicc -o $@ $^ -std=c99 -lm -g
