#!/bin/sh
module load mpi/openmpi-x86_64
make

for i in 1 2 4 8 16
do
    for j in 1 2 4 8 16
        do
            if [ $((i*j)) -le 16 ]
            then
                printf "\nTime for MPI+Pthreads Implementation with $i process(es) each with $j thread(s)\n"
                time mpirun --mca btl ^openib -np $i ./game_of_life input_10000_x_20000.txt $j
            fi   
        done
done

make clean