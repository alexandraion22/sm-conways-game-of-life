#!/bin/sh
make

for i in 1 2 4 8 16
do
    printf "\nTime for Pthreads Implementation with $i thread(s)\n"
    time ./game_of_life input_10000_x_20000.txt $i
done

make clean