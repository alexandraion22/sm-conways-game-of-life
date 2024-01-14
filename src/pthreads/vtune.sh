#!/bin/sh

mkdir pthreads_hotspots
mkdir pthreads_performance-snapshot
mkdir pthreads_threading

make

for i in 1 2 4 8 16
do
    for j in hotspots performance-snapshot threading
    do
        vtune -collect $j -r result_dir_pthreads_${i}${j} ./game_of_life input_10000_x_20000.txt $i > pthreads_${j}/vtune_threads${i}
    done
done 

make clean