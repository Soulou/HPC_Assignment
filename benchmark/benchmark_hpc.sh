#!/bin/bash

# $1 -> executable
executable=$1
# $2 -> N
N=$2

for n in 1 2 4 8 16 32; do
  sub=$1_$n.sub
  cp -v laplace_mpi.sub $sub
  sed -i "s/leo-laplace-mpi/leo-laplace-mpi-$n/" $sub
  sed -i "s/\.\/laplace_mpi/$1/" $sub
  sed -i "s/N w t/$N 0.2 0.0001/" $sub
  if "$n" -gt 16 ; then
    sed -i "s/select=1/select=2/" $sub
  else
    sed -i "s/select=2/select=1/" $sub
  fi
  qsub $sub
  # rm -v $sub
done
~               
