#!/bin/bash

# $1 -> executable
executable=$1
# $2 -> N
$N = $2

for n in 32; do
  sub=$(basename $1)$N_$n.sub
  cp -v laplace_mpi.sub $sub
  echo "Change name"
  sed -i "s/leo-laplace-mpi/leolaplacempi$N_$n/" $sub
  echo "Change exec"
  sed -i "s+\./laplace_mpi+$executable+" $sub
  echo "Change params"
  sed -i "s/N w t/$N 0.2 0.000001/" $sub
  if [ "$n" -gt 16 ] ; then
    sed -i "s/select=1/select=2/" $sub
  else
    sed -i "s/ncpus=16:mpiprocs=16/ncpus=$n:mpiprocs=$n/" $sub
  fi
  qsub $sub
  rm -v $sub
done
