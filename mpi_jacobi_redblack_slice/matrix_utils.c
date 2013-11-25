#include "matrix_utils.h"

double f(int i, int N) {
  return pow(sin(PI * i * 1.0/(N+1)), 2);
}

double ** init_matrix_from_array(double * a, int n) {
	double ** res = (double **)malloc(sizeof(double *) * n);
	for(int i = 0; i < n; i++) {
		res[i] = (double *)calloc(n, sizeof(double));
		for(int j = 0; j < n; j++) {
			if(j == 0)
				res[i][j] = f(i, n);
			else if(j == n-1 || i == 0 || i == n-1)
				res[i][j] = 0.0;
			else {
				res[i][j] = a[(i-1)*n + j];
			}
		}
	}
	return res;
}	

void set_line(double ** matrix, int n, int m, int i, double v) {
	if(i >= n) { printf("Invalid line %d, greater that matrix %d\n", i, n); return; }
	for(int j = 1; j < m; j++) {
		matrix[i][j] = v;
	}
}

/*
 * Create matrix, partition of the domain
 *
 * |f             0|
 * |f             0|
 * |f      w      0|
 * |f             0|
 * |f             0|
 *
 */
double ** init_matrix(int n, int m, double w, int rank, int nb_nodes) {
	double ** matrix = (double **)malloc(sizeof(double *) * n);
	for(int i = 0; i < n; i ++) {
		matrix[i] = (double *)calloc(m, sizeof(double));
		for(int j = 0; j < m; j++) {
			if(j == 0)
				matrix[i][j] = f(i + rank*(n-1), m);
			else if(j == m-1)
				matrix[i][j] = 0.0;
			else
				matrix[i][j] = w;
		}
	}
	// Initialize borders for x = 0 and x = N - 1
	if(rank == 0)
		set_line(matrix, n, m, 0, 0.0);
	if(rank == nb_nodes-1)
		set_line(matrix, n, m, n-1, 0.0);
	return matrix;
}

void free_matrix(double ** m, int n) {
	for(int i = 0; i < n ; i++) {
		free(m[i]);
	}
	free(m);
}

double ** diff_matrices(double ** m1, double ** m2, int n, int m) {
	double ** res = (double **)malloc(sizeof(double *) * n);
	for(int i = 0; i < n; i++) {
		res[i] = (double *)malloc(sizeof(double) * m);
		for(int j = 0; j < m ; j++)
			res[i][j] = m1[i][j] - m2[i][j];
	}
	return res;
}

double error(double ** old_m, double ** new_m, int n, int m) {
	double ** diff = diff_matrices(new_m, old_m, n, m);
	double res = NORM(diff, n, m);
	free_matrix(diff, n);
	return res;
}
