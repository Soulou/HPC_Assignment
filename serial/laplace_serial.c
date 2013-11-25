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
#include "matrix_norms.h"

#define NORM two_norm
#define PI 3.141592653589793238462643383279502884L

double f(int i, int n) {
  return pow(sin(PI * i * 1.0/(n+1)), 2);
}

double ** init_matrix(int n, double w) {
	double ** matrix = (double **)malloc(sizeof(double *) * (n + 2));
	for(int i = 0; i < n; i ++) {
		matrix[i] = (double *)calloc(sizeof(double), n + 2);
		for(int j = 0; j < n - 1 ; j++) {
			if(j == 0)
				matrix[i][j] = f(i, n);
			else if(i == n-1 || i == 0)
				matrix[i][j] = 0;
			else
				matrix[i][j] = w;
		}
	}
	return matrix;
}

void print_data_matrix(double ** matrix, int n) {
	FILE * results = fopen("laplace_results.txt", "w");
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			fprintf(results, "%1.10lf %1.10lf %1.10lf\n", (double)i/(n-1.), (double)j/(n-1.), matrix[j][i]);
			/* printf("%1.10lf\n", matrix[i][j]); */
		}
	}
	fclose(results);
}

void print_matrix(double ** matrix, int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			printf("%1.10lf ", matrix[i][j]);
		}
		printf("\n");
	}
}

void free_matrix(double ** m, int n) {
	for(int i = 0; i < n ; i++) {
		free(m[i]);
	}
	free(m);
}

double ** diff_matrices(double ** m1, double ** m2, int n) {
	double ** res = (double **)malloc(sizeof(double *) * n);
	for(int i = 0; i < n; i++) {
		res[i] = (double *)malloc(sizeof(double) * n);
		for(int j = 0; j < n ; j++)
			res[i][j] = m1[i][j] - m2[i][j];
	}
	return res;
}

double ** iterate(double ** matrix, int n) {
	double ** matrix2 = init_matrix(n, 0.0);
	for(int i = 1; i < n-1; i++) {
		for(int j = 1; j < n-1; j++) {
			matrix2[i][j] = (matrix[i-1][j] + matrix[i+1][j] + matrix[i][j-1] + matrix[i][j+1]) * 0.25;
		}
	}
	return matrix2;
}

double error(double ** old, double ** new, int n) {
	double ** diff = diff_matrices(new, old, n);
	double res = NORM(diff, n);
	free_matrix(diff, n);
	return res;
}

double convergence_rate(double ** oldold, double ** old, double ** new, int n) {
	/* printf("NORM(new): %0.10lf\n", NORM(new, n)); */
	/* printf("NORM(old): %0.10lf\n", NORM(old, n)); */
	/* printf("NORM(oldold): %0.10lf\n", NORM(oldold, n)); */
	return error(old, new, n) / error(oldold, old, n);
}

int main(int argc, char * argv[])
{
	int n = atoi(argv[1]);
	double w = atof(argv[2]);
	double e = atof(argv[3]);

	FILE * f_error = fopen("./laplace_error.txt", "w");
	FILE * f_convergence_rate = fopen("./laplace_convergence_rate.txt", "w");

	if(argc != 4) {
		printf("Usage: %s <N> <init-value> <error>", argv[0]);
		return 1;
	}

	double ** new_m = init_matrix(n, w);
	double ** prev_m, ** prev_prev_m;
	double it_error = 0.0;

	int nb_iterations = 0;
	do {
		nb_iterations++;
		prev_prev_m = prev_m;
		prev_m = new_m;
		new_m = iterate(prev_m, n);
	  it_error = error(prev_m, new_m, n);
		fprintf(f_error, "%d %lf\n", nb_iterations, it_error);
		if(nb_iterations > 2)
			fprintf(f_convergence_rate, "%d %lf\n", nb_iterations, convergence_rate(prev_prev_m, prev_m, new_m, n));
	} while(it_error > e);
	// print_matrix(new_m, n);
	printf("Norm(m) = %lf\n", NORM(new_m, n));
	print_data_matrix(new_m, n);
	fprintf(stderr, "Number of iterations: %d\n", nb_iterations);

	fclose(f_error);
	fclose(f_convergence_rate);
	return 0;
}

