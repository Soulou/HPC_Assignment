all: laplace_serial

laplace_serial: laplace_serial.c matrix_norms.c
	gcc -o $@ $^ -std=c99 -lm
