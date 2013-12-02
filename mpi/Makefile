all: mpi_gs_redblack_slice mpi_gs_slice mpi_jacobi_redblack_slice

mpi_gs_redblack_slice:
	$(MAKE) -C $@
	mv $@/laplace_mpi $@_bin

mpi_gs_slice:
	$(MAKE) -C $@
	mv $@/laplace_mpi $@_bin

mpi_jacobi_redblack_slice:
	$(MAKE) -C $@
	mv $@/laplace_mpi $@_bin
