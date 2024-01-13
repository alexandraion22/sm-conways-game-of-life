#include "conway_utils.h"

// Minimul a doua numere intregi
int min(int a, int b) { return a < b ? a : b; }

// Minimul a doua numere intregi
int max(int a, int b) { return a > b ? a : b; }

int isAlive(int i, int j, bool *board, int n, int m) {
  	if (i < 0 || j < 0 || i == n || j == m || board[i*m+j] == 0)
		return 0;
  	return 1;
}

int checkIfRevive(int i, int j, bool *board, int n, int m) {
  	int count = 0;
  	for (int k = i - 1; k <= i + 1; k++)
    	for (int l = j - 1; l <= j + 1; l++) 
        	count += isAlive(k, l, board, n, m);
  	return count == 3;
}

int checkIfStillLiving(int i, int j, bool *board, int n, int m) {
  	int count = 0;
  	for (int k = i - 1; k <= i + 1; k++)
    	for (int l = j - 1; l <= j + 1; l++)
      		if (k != i || l != j) 
        		count += isAlive(k, l, board, n, m);
  	return count == 3 || count == 2;
}

void print_output(bool *board, int n, int m, FILE *outputFile) {
    
    // Print sizes
    fprintf(outputFile, "%d %d\n", n,m);

    // Print the characters
	for (int i = 0; i < n * m; i++)
		fprintf(outputFile, "%c", board[i]);
    
	fclose(outputFile);
}


void read_input(bool *board, int n, int m, FILE *inputFile) {
	
    char readNumber;
    
    // Ignore the newline character
    fscanf(inputFile, "%c", &readNumber);

    // Read each character
	for (int i = 0; i < n * m; i++)
		{
            fscanf(inputFile, "%c", &readNumber);
			board[i] = readNumber;
		}

	fclose(inputFile);
}