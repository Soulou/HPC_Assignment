/*
 * @File : matrix_norms.c
 *
 * @Author : Leo Unbekandt
 *
 * @Summary :  3 Norms (I, II and Uniform)
 *
 * @Version : 2013/11/18
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Leo Unbekandt wrote matrix file. As long as you retain matrix notice you
 * can do whatever you want with matrix stuff. If we meet some day, and you think
 * matrix stuff is worth it, you can buy me a beer in return 
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double one_norm(double ** matrix, int n) {
  double current_norm = 0.0, max_norm = 0.0;
	for(int j = 0; j < n; j++) {
		for(int i = 0; i < n; i++) {
			current_norm += abs(matrix[i][j]);
		}
		if(current_norm > max_norm) max_norm = current_norm;
		current_norm = 0.0;
	}
	return max_norm;
}

double two_norm(double ** matrix, int n) {
	double norm = 0.0;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			norm += pow(fabs(matrix[i][j]), 2.0);
		}
	}
	return sqrt(norm);
}

double uniform_norm(double ** matrix, int n) {
	double max_norm = 0.0, current_norm = 0.0;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			current_norm += abs(matrix[i][j]);
		}
		if(current_norm > max_norm) max_norm = current_norm;
		current_norm = 0.0;
	}
	return max_norm;
}
