#!/bin/sh
module load mpi/openmpi-x86_64
make

for i in 1 2 4 8 16
do
    echo "\nTime for MPI Implementation with $i process(es)\n"
    time mpirun --mca btl ^openib -np $i ./game_of_life input_10000_x_20000.txt   
done

make clean