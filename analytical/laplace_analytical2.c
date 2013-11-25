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
#include <gmp.h>
#include <mpfr.h>
#include <math.h>

#define PI 3.141592653589793238462643383279502884L
#define INFINITE 30

void print_data_matrix(mpfr_t ** matrix, int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			printf("%1.10lf %1.10lf ", (double)i/(n-1.), (double)j/(n-1.));
			mpfr_printf("%0.30RNf\n", matrix[i][j]);
		}
	}
}

mpfr_t ** init_matrix(int n) {
	mpfr_t ** m = (mpfr_t **)malloc(n * sizeof(mpfr_t *));
	for(int i = 0; i < n; i++) {
		m[i] = (mpfr_t *)malloc(n * sizeof(mpfr_t));
	}
	return m;
}

void an(mpfr_t res, int n) {
	double nd = (double)n;
	mpfr_t tmp, tmp2;

	// 4.0 * (cos(PI * nd) -1)
	mpfr_init_set_d(res, nd * PI, MPFR_RNDN);
	mpfr_cos(res, res, MPFR_RNDN);
	mpfr_sub_d(res, res, 1.0, MPFR_RNDN);
	mpfr_mul_d(res, res, 4.0, MPFR_RNDN);

	// PI * (pow(nd, 3.0) - 4.0 * nd)
	mpfr_init_set_d(tmp, nd, MPFR_RNDN);
	mpfr_pow_ui(tmp, tmp, 3, MPFR_RNDN);
	mpfr_sub_d(tmp, tmp, 4.0 * nd, MPFR_RNDN);
	mpfr_mul_d(tmp, tmp, PI, MPFR_RNDN);

	// sinh(-nd * PI)
	mpfr_init_set_d(tmp2, -nd * PI, MPFR_RNDN);
	mpfr_sinh(tmp2, tmp2, MPFR_RNDN);

	// Final res
	mpfr_mul(tmp, tmp, tmp2, MPFR_RNDN);
	mpfr_div(res, res, tmp, MPFR_RNDN);
	
	mpfr_clear(tmp);
	mpfr_clear(tmp2);

	if(mpfr_nan_p(res))
		mpfr_set_zero(res, 1);
}

int main(int argc, char * argv[])
{
	if(argc != 2) {
		printf("Usage: %s <N>\n", argv[0]);
		return 1;
	}

	int N = atoi(argv[1]);
	double x, y;
	double nd;

	mpfr_t ** phi = init_matrix(N);
	mpfr_t sum, tmp_sum;
	mpfr_init(sum);
	mpfr_init(tmp_sum);
	mpfr_t * a  = (mpfr_t *)malloc(sizeof(mpfr_t) * INFINITE);
	for(int i = 1; i < INFINITE ; i++) {
		an(a[i-1], i);
	}

	for(int i = 0; i < N; i++) {
		x = (double)i/(N-1);
		for(int j = 0; j < N; j++) {
			y = (double)j/(N-1);
			mpfr_set_zero(sum, 1);
			for(int n = 1; n < INFINITE; n++) {
				nd = (double)n;
				mpfr_set_d(tmp_sum, nd * PI * (x - 1.0), MPFR_RNDN);
				mpfr_sinh(tmp_sum, tmp_sum, MPFR_RNDN);
				mpfr_mul(tmp_sum, tmp_sum, a[n-1], MPFR_RNDN);
				mpfr_mul_d(tmp_sum, tmp_sum, sin(nd * PI * y), MPFR_RNDN);
				mpfr_add(sum, sum, tmp_sum, MPFR_RNDN);
			}
			mpfr_init_set(phi[i][j], sum, MPFR_RNDN);
		}
	}
	print_data_matrix(phi, N);

	mpfr_clear(sum);
	mpfr_clear(tmp_sum);
	return 0;
}

