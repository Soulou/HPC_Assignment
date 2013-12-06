#!/bin/bash

for n in 20 40 60 80 100 ; do
  for t in 0.01 0.001 0.0001 0.00001 ; do
    ./analytical/laplace_analytical $n
    mv laplace_results.txt a.txt
    ./serial/laplace_serial $n 0.2 $t
    mv laplace_results.txt b.txt
    ruby diff_results.rb a.txt b.txt > c.txt
    echo "$n $t $(cat c.txt)" >> diff_results.txt
  done
done
  
rm a.txt b.txt c.txt laplace_results.txt

