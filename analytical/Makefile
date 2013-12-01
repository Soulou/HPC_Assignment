all: laplace_analytical

laplace_analytical: laplace_analytical.c
	gcc -o $@ $< -g -lm --std=c99 -lmpfr -lgmp
