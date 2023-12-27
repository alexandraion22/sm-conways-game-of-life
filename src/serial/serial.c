#include "../utils/conway_utils.h"

void gameOfLife(bool **board, int n, int m)
{
  	bool **resBoard = (bool **)malloc(n * sizeof(bool *));
	for (int i = 0; i < n; i++)
		resBoard[i] = (bool *)malloc(m * sizeof(bool));

  	for (int i = 0; i < n; i++)
    	for (int j = 0; j < m; j++)
      		if (board[i][j] == 0) {
        		if (checkIfRevive(i, j, board, n, m))
          			resBoard[i][j] = 1;
        		else
          			resBoard[i][j] = 0;
      		} else {
        		if (checkIfStillLiving(i, j, board, n, m))
          			resBoard[i][j] = 1;
        		else
          			resBoard[i][j] = 0;
      		}
  	
	// Eliberare memorie si salvare rezultat
	for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++) 
        		board[i][j] = resBoard[i][j];
			free(resBoard[i]);
		}
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
	sprintf(fileNameOut, "../../output/serial/result_of_%s", argv[1]);

	FILE *inputFile = fopen(fileNameIn, "r");
	FILE *outputFile = fopen(fileNameOut, "w");

	if (inputFile == NULL || outputFile == NULL) {
		perror("Error opening file");
		return 1;
	}

	// Alocare spatiu matrice de intrare
	int n, m;

	fscanf(inputFile, "%d %d", &n, &m);

	bool **board = (bool **)malloc(n * sizeof(bool *));
	for (int i = 0; i < n; i++)
			board[i] = (bool *)malloc(m * sizeof(bool));

	// Citire date matrice de intrare
	read_input(board,n,m,inputFile);

	gameOfLife(board, n, m);

	// Salvare date in fisierul de iesire
	print_output(board,n,m,outputFile);
	
	// Eliberare memorie
	for (int i = 0; i < n; i++)
		free(board[i]);
	free(board);

	return 0;
}
