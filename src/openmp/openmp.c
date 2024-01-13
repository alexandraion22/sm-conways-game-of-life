#include "../utils/conway_utils.h"
#include <omp.h>

void gameOfLife(bool *board, int n, int m)
{
  	bool *resBoard = (bool *)malloc(n * m * sizeof(bool));
	int i,j;

	// Paralelizare utilizand openmp
	#pragma omp parallel for private(i, j) shared(resBoard) collapse(2)
  	for (i = 0; i < n; i++)
    	for (j = 0; j < m; j++)
      		if (board[i*m+j] == 0) {
        		if (checkIfRevive(i, j, board, n, m))
          			resBoard[i*m+j] = 1;
        		else
          			resBoard[i*m+j] = 0;
      		} else {
        		if (checkIfStillLiving(i, j, board, n, m))
          			resBoard[i*m+j] = 1;
        		else
          			resBoard[i*m+j] = 0;
      		}
  	
	// Eliberare memorie si salvare rezultat
	memcpy(board,resBoard,(m*n)*sizeof(bool));
	free(resBoard);
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Wrong number of arguments. The use of the program is ./game_of_life <input_file>\n");
		return 0;
	}

	// Initializare fisiere de intrare/iesire
	char fileNameIn[100];
	char fileNameOut[100];
	sprintf(fileNameIn, "../../input/%s", argv[1]);
	sprintf(fileNameOut, "../../output/openmp/result_of_%s", argv[1]);

	FILE *inputFile = fopen(fileNameIn, "r");
	FILE *outputFile = fopen(fileNameOut, "w");

	if (inputFile == NULL || outputFile == NULL) {
		perror("Error opening file");
		return 1;
	}

	// Alocare spatiu matrice de intrare
	int n, m;

	fscanf(inputFile, "%d %d", &n, &m);

	bool *board = (bool *)malloc(n * m * sizeof(bool));

	// Citire date matrice de intrare
	read_input(board,n,m,inputFile);

	gameOfLife(board, n, m);

	// Salvare date in fisierul de iesire
	print_output(board,n,m,outputFile);
	
	// Eliberare memorie
	free(board);

	return 0;
}