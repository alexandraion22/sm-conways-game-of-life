#include "conway_utils.h"

int isAlive(int i, int j, bool **board, int n, int m) {
  	if (i < 0 || j < 0 || i == n || j == m || board[i][j] == 0)
		return 0;
  	return 1;
}

int checkIfRevive(int i, int j, bool **board, int n, int m) {
  	int count = 0;
  	for (int k = i - 1; k <= i + 1; k++)
    	for (int l = j - 1; l <= j + 1; l++) 
        	count += isAlive(k, l, board, n, m);
  	return count == 3;
}

int checkIfStillLiving(int i, int j, bool **board, int n, int m) {
  	int count = 0;
  	for (int k = i - 1; k <= i + 1; k++)
    	for (int l = j - 1; l <= j + 1; l++)
      		if (k != i || l != j) 
        		count += isAlive(k, l, board, n, m);
  	return count == 3 || count == 2;
}

void print_output(bool **board, int n, int m, FILE *outputFile) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			fprintf(outputFile, "%d ", board[i][j]);
		fprintf(outputFile, "\n");
	}
	fclose(outputFile);
}


void read_input(bool **board, int n, int m, FILE *inputFile) {
	int readNumber;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			{
				fscanf(inputFile, "%d", &readNumber);
				board[i][j] = readNumber;
			}
	fclose(inputFile);
}