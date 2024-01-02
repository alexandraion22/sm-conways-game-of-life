#!/bin/sh
make

for i in 1 2 4 8 16
do
    export OMP_NUM_THREADS=$i
    printf "\nTime for OpenMP Implementation with $i thread(s)\n"
    time ./game_of_life input_10000_x_20000.txt
done

export OMP_NUM_THREADS=

make clean