#!/bin/bash

# $1 -> binary

executable=$1

for N in 66 126 256 516 1026 ; do
  for n in 1 2 4 8 16 32; do
    mpirun -n $n $executable $N 0.2 0.001
  done
done
