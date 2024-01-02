#!/bin/sh
module load mpi/openmpi-x86_64
make

for i in 1 2 4 8 16
do
    for j in 1 2 4 8 16
        do
            if [ $((i*j)) -le 16 ]
            then
                export OMP_NUM_THREADS=$i
                printf "\nTime for MPI+OpenMP Implementation with $i process(es) each with $j thread(s)\n"
                time mpirun --mca btl ^openib -np $i ./game_of_life input_10000_x_20000.txt $j
            fi   
        done
done

export OMP_NUM_THREADS=

make clean