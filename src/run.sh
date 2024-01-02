#!/bin/sh
cd pthreads

./time.sh

cd ../openmp

./time.sh

cd ../mpi

./time.sh

cd ../mpi_pthreads

./time.sh

cd ../mpi_openmp

./time.sh