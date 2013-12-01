#ifndef _MATRIX_UTILS_H
#define _MATRIX_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix_norms.h"

#define NORM two_norm
#define PI 3.141592653589793238462643383279502884L

double f(int i, int N);
double ** init_matrix_from_array(double * a, int n);
void set_line(double ** matrix, int n, int m, int i, double v);
double ** init_matrix(int n, int m, double w, int rank, int nb_nodes);
void free_matrix(double ** m, int n);
double ** copy_matrix(double ** matrix, int n, int m);
double **	diff_matrices(double ** m1, double ** m2, int n, int m);
double error(double ** old_m, double ** new_m, int n, int m);

#endif
