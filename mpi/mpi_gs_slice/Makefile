all: laplace_mpi

laplace_mpi: laplace_mpi.c ../common/matrix_norms.c ../common/matrix_utils.c ../common/output.c
	mpicc -o $@ $^ -std=c99 -lm -g
