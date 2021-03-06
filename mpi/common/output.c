#include "output.h"

void print_data_matrix(char * exec, int p, double ** matrix, int n) {
	char buffer[256];
	sprintf(buffer, "%s_P%d_N%d", exec, p, n);
	FILE * f = fopen(buffer, "w");
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			fprintf(f, "%1.10lf %1.10lf %1.30lf\n", (double)i/(n-1.), (double)j/(n-1.), matrix[j][i]);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

void print_matrix(double ** matrix, int n, int m) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			printf("%1.10lf ", matrix[i][j]);
		}
		printf("\n");
	}
}

void print_array(double * array, int n) {
	for(int i = 0; i < n; i++) {
		printf("%1.10lf ", array[i]);
	}
	printf("\n");
}

