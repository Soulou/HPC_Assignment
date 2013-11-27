/*
 * @File : laplace_analytical.c
 *
 * @Author : Leo Unbekandt
 *
 * @Summary : Laplce theoretical algorithm
 *
 * @Version : 2013/11/18
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Leo Unbekandt wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return 
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793238462643383279502884L
#define INFINITE 30

void print_data_matrix(double ** matrix, int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			printf("%1.10lf %1.10lf %1.10lf\n", (double)i/(n-1.), (double)j/(n-1.), matrix[j][i]);
			/* printf("%1.10lf\n", matrix[i][j]); */
		}
		printf("\n");
	}
}

double ** init_matrix(int n) {
	double ** m = (double **)malloc(n * sizeof(double *));
	for(int i = 0; i < n; i++) {
		m[i] = (double *)malloc(n * sizeof(double));
	}
	return m;
}

double an(int n) {
	double nd = (double)n;
	long double res = 4.0 * (cos(PI * nd) - 1 ) / (PI * (pow(nd, 3.0)- 4.0 * nd) * sinh(-nd * PI));
	printf("An n=%d: %0.10llf\n", n, res);
	return (double)res;
}

int main(int argc, char * argv[])
{
	if(argc != 2) {
		printf("Usage: %s <N>\n", argv[0]);
		return 1;
	}

	int N = atoi(argv[1]);
	double * a = (double *)malloc(sizeof(double) * INFINITE);
	for(int i = 0; i < INFINITE; i++) {
		a[i] = an(i);
	}

	double ** phi = init_matrix(N);
	double sum, x, y;
	for(int i = 0; i < N; i++) {
		x = (double)i/(N-1);
		for(int j = 0; j < N; j++) {
			y = (double)j/(N-1);
			sum = 0.0;
			for(int n = 1; n < INFINITE; n++) {
				sum += a[n] * (sinh((double)n * PI * x) - tanh((double)n * PI) * cosh((double)n * PI * x)) * sin((double)n * PI * y); 
			}
		}
	}
	/* print_data_matrix(phi, N); */
	return 0;
}

