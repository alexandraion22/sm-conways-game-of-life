#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int isAlive(int i, int j, bool **board, int n, int m);
int checkIfRevive(int i, int j, bool **board, int n, int m);
int checkIfStillLiving(int i, int j, bool **board, int n, int m);
void print_output(bool **board, int n, int m, FILE *outputFile);
void read_input(bool **board, int n, int m, FILE *inputFile);
