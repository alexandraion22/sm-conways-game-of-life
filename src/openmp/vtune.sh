#!/bin/sh

mkdir openmp_hotspots
mkdir openmp_performance-snapshot
mkdir openmp_threading

make

for i in 1 2 4 8 16
do
    for j in hotspots performance-snapshot threading
    do
        export OMP_NUM_THREADS=$i
        vtune -collect $j -r result_dir_openmp_${i}${j} ./game_of_life input_10000_x_20000.txt > openmp_${j}/vtune_threads${i}
    done
done 

export OMP_NUM_THREADS=

make clean
