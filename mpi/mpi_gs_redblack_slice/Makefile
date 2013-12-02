all: laplace_mpi

laplace_mpi: laplace_mpi.c ../common/output.c ../common/matrix_utils.c ../common/matrix_norms.c
	mpicc -o $@ $^ -std=c99 -lm -g
