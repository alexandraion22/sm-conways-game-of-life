#include <stdio.h>
#include <stdlib.h>

int isAlive(int i, int j, int **board, int n, int m) {
  if (i < 0 || j < 0 || i == n || j == m || board[i][j] == 0) return 0;
  return 1;
}

int checkIfRevive(int i, int j, int **board, int n, int m) {
  int count = 0;
  for (int k = i - 1; k <= i + 1; k++)
    for (int l = j - 1; l <= j + 1; l++) 
        count += isAlive(k, l, board, n, m);
  return count == 3;
}

int checkIfStillLiving(int i, int j, int **board, int n, int m) {
  int count = 0;
  for (int k = i - 1; k <= i + 1; k++)
    for (int l = j - 1; l <= j + 1; l++)
      if (k != i || l != j) 
        count += isAlive(k, l, board, n, m);
  return count == 3 || count == 2;
}

void gameOfLife(int **board, int boardSize, int boardColSize) {
  int n = boardSize;
  int m = boardColSize;
  int resBoard[n][m];
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
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) 
        board[i][j] = resBoard[i][j];
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf(
        "Wrong number of arguments. The use of the program is ./game_of_life "
        "<input_file>\n");
    return 0;
  }

  char fileNameIn[100];
  char fileNameOut[100];
  sprintf(fileNameIn, "../../input/%s", argv[1]);
  sprintf(fileNameOut, "../../output/result_of_%s", argv[1]);

  FILE *inputFile = fopen(fileNameIn, "r");
  FILE *outputFile = fopen(fileNameOut, "w");

  if (inputFile == NULL || outputFile == NULL) {
    perror("Error opening file");
    return 1;
  }

  int n, m;
  fscanf(inputFile, "%d %d", &n, &m);
  int **board = (int **)malloc(n * sizeof(int *));
  for (int i = 0; i < n; i++) 
    board[i] = (int *)malloc(m * sizeof(int));

  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) 
        fscanf(inputFile, "%d", &board[i][j]);

  gameOfLife(board, n, m);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) 
        fprintf(outputFile, "%d ", board[i][j]);
    fprintf(outputFile, "\n");
  }

  for (int i = 0; i < n; i++) 
    free(board[i]);
  free(board);

  fclose(inputFile);
  fclose(outputFile);
  
  return 0;
}
