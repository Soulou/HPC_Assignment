High Performance Computing
==========================

Solving Laplace Equation
------------------------

### Analytical Solution

The analytical solution is using Fourier series to discretize the result.
Technically, it has been implemented with the MPFR lib to be able to manipulate
high precision numbers. It is necessary because of the calls to __sinh__ which
generated values which are not storeable in a _long double_

Usage:
```
# N: Edge size of the matrix
./laplace_analytical <N>
```

### Serial Solution (with Jacobi)

The results are based on 5-points finite different numerical solution to Laplace
equation.

Usage:
```
# N: Edge size of the matrix
# w: Init value
# e: Tolerance to check convergence
./laplace_serial <N> <w> <e>
```

### MPI Solutions

Build:
```
cd mpi
make -B
```

Usage:
```
# N: Edge size of the matrix
# w: Init value
# e: Tolerance to check convergence
./mpi_gs_redblack_slice_bin <N> <w> <e>
./mpi_jacobi_redblack_slice_bin <N> <w> <e>
./mpi_gs_slice_bin <N> <w> <e>
```

> Jacobi and domain horizontal slicing
> Red-Black Ordering Jacobi and domain horizontal slicing
