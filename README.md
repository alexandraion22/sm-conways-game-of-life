# Conway's Game of Life

"Game of Life", cunoscut și sub numele de "Life", este un automat celular conceput de matematicianul britanic John Horton Conway în 1970. Este un **joc fără jucători**, ceea ce înseamnă că evoluția sa este determinată de starea sa inițială, nefiind nevoie de alte informații. Se interacționează cu Game of Life prin a crea o configurație inițială și observând cum evoluează.

Am preluat ideea de problemă de pe Leetcode, unde am întâlnit-o prima dată. Problema este la bază o problemă **Embarrasingly Parallel**, dat fiind faptul ca valorile noilor celule depind doar de valorile vechilor celule.

## Echipa

- Ciobanu Daria 342C1
- Ion Alexandra Ana-Maria 341C1

## Cod

### Rulare Cod

#### Serial

    cd ./src/serial
    make
    ./game_of_life <Nume_Fisier>

#### Pthreads

    cd ./src/pthread
    make
    ./game_of_life <Nume_Fisier> <Numar_thread-uri>

#### OpenMP

    cd ./src/openmp
    make
    export OMP_NUM_THREADS=<Numar_thread-uri>
    ./game_of_life <Nume_Fisier>

#### MPI

    cd ./src/mpi
    make
    mpirun -np <Numar_thread-uri> ./game_of_life <Nume_Fisier>

## Referinte

1. [Leetcode - Game of Life](https://leetcode.com/problems/game-of-life/)
2. [Wikipedia - Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

## TODO

    - adaugat Makefile in variante paralele - Daria
    - adaugat MPI - Alexandra
    - adaugat OpenMP - Daria
    - adaugat pthreads - Daria
    - adaugat script generare input
