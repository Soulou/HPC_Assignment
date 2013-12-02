/*
 * @File : main.c
 *
 * @Author : Leo Unbekandt
 *
 * @Summary : Laplace Equation Solver -> Linear
 *
 * @Version : 2013/11/11
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Leo Unbekandt wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return 
 * ----------------------------------------------------------------------------
 */

/*
 * Convergence:
 * || phi_i+1 - phi_i || < \epsilon
 * || m || -> Norme
 * Taux de convergence:
 * || phi_i+1 - phi_i ||
 * ---------------------
 * || phi_i - phi_i-1 ||
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include "../common/matrix_norms.h"
#include "../common/matrix_utils.h"
#include "../common/output.h"

/*
 * @param old_m The data comes from this matrix
 * @param new_m We are filling this matrix
 * @param n Dimension 1 of the matrices
 * @param m Dimension 2 of the matrices
 * @brief Do an iteration of Jacobi
 */
void compute(double ** matrix, int n, int m) {
	for(int i = 1; i < n-1; i++) {
		for(int j = 1; j < m-1 ; j++) {
			matrix[i][j] = (matrix[i-1][j] + matrix[i+1][j] + matrix[i][j-1] + matrix[i][j+1]) * 0.25;
		}
	}
}

/*
 * @param matrix Subject of the halo exchange
 * @param n Dimension 1 of matrix
 * @param m Dimension 2 of matrix
 * @param rank MPI rank of the process
 * @param nb_nodes Total number of nodes
 * @brief Exchange the halo data of the different subdomaines
 * 
 * _______________________
 * |  0      --       0   |
 * |                      |
 * | ʌ  ʌ  ʌ  ʌ  ʌ  ʌ  ʌ  |
 * |-|--|--|--|--|--|--|--|
 * | v  v  v  v  v  v  v  |
 * |                      |
 * | ʌ  ʌ  ʌ  ʌ  ʌ  ʌ  ʌ  |
 * |-|--|--|--|--|--|--|--|
 * | v  v  v  v  v  v  v  |
 * |                      |              
 * |  0      --       0   |
 * ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
 *
 */
void exchange_halo(double ** matrix, int n, int m, int rank, int nb_nodes) {
	MPI_Status statuses[4];
	MPI_Request requests[4];
	for(int i = 0; i < 4; i++) {
		requests[i] = MPI_REQUEST_NULL;
	}

	// To test: matrix[n-1]+1, m-2 to send the minimum
	if(rank != 0)
		MPI_Irecv(matrix[0], m, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &requests[2]);
	if(rank != nb_nodes-1)
		MPI_Irecv(matrix[n-1], m, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &requests[3]);
	if(rank != 0)
		MPI_Isend(matrix[1], m, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &requests[0]);
	if(rank != nb_nodes-1)
		MPI_Isend(matrix[n-2], m, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &requests[1]);

	MPI_Waitall(4, requests, statuses);
}

int main(int argc, char * argv[])
{
	// First, we initialize MPI
	int rank, nb_nodes;
	int return_code;
	return_code = MPI_Init(&argc, &argv);
	if(return_code != MPI_SUCCESS) {
		printf("Fail to initialize MPI\n");
		MPI_Abort(MPI_COMM_WORLD, return_code);
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nb_nodes);

	// Check the arguments
	if(!rank && argc != 4) {
		printf("Usage: %s <N> <init-value> <error>", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	// Setup arguments (number of columns, initial value, error tolerance)
	// n et m are the dimension of the subdomains, we keep the size of the global matrix in N
	int n = atoi(argv[1]);
	int N = n;
	int m = n;
	n = (n-2)/nb_nodes + 2;
	double w = atof(argv[2]);
	double e = atof(argv[3]);
	
	// Check that p divides N-2. Why '-2' because we don't care of the top and bottom lines of 0
	if(!rank && (N-2) % nb_nodes != 0) {
		printf("The number of processors must divide the size of the matrix - 2\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	// Compute matrices
	double ** prev_m = NULL;
	double ** new_m = init_matrix(n, m, w, rank, nb_nodes);

	// Local and global error
	double it_error = 0.0;
	double global_error = 0.0;

	double t1, t2;
	t1 = MPI_Wtime();

	int itnb = 0;
	do {
		itnb++;
		if(prev_m != NULL)
			free_matrix(prev_m, n);

		prev_m = new_m;
		new_m = copy_matrix(prev_m, n, m);
		compute(new_m, n, m);
		exchange_halo(new_m, n, m, rank, nb_nodes);
		
		/* compute_red(new_m, n, m); */
		/* exchange_red(new_m, n, m, rank, nb_nodes); */
		/* compute_black(new_m, n, m); */
		/* exchange_black(new_m, n, m, rank, nb_nodes); */

		// Every 30 iterations we check the convergence
		if(itnb % 30 == 0) {
			it_error = error(prev_m, new_m, n, m);
			MPI_Reduce(&it_error, &global_error, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
			MPI_Bcast(&global_error, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			if(global_error <= e)
				break;
		}
	} while(1);

	// We need to gather all the data to node 0
	// Number of lines - halo data * number of columns
	int send_size = (n-2) * m;
	int result_size = send_size * nb_nodes;
	double * send = (double*)malloc(sizeof(double) * send_size);
	double * result;
	if(!rank) {
		printf("Iterations number: %d\n", itnb);
		result = (double *)malloc(sizeof(double) * result_size);
	}

	// Some DEBUG prints
	/* printf("End partial matrix of %d\n", rank); */
	/* print_matrix(new_m, n, m); */

	for(int i = 1; i < n-1; i++)
		for(int j = 0; j < m; j++)
			send[(i-1)*m+j] = new_m[i][j];

	t2 = MPI_Wtime();
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(send, send_size, MPI_DOUBLE, result, send_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	free(send);

	// The node 0 builds the global matrix and print the data
	if(!rank) {
		printf("Execution Time: %1.2lf\n", t2-t1);
		/* printf("Result\n"); */
		/* print_array(result, result_size); */
		double ** global_matrix = init_matrix_from_array(result, N);
		free(result);
		print_data_matrix(argv[0], nb_nodes, global_matrix, N);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

