#include "../utils/conway_utils.h"
#include <pthread.h>

typedef struct {
    int startRow;
    int endRow;
    int n;
    int m;
    bool *board;
    bool *resBoard;
} thread_arg;

void *gameOfLifeThread(void *arg) {

	// Initalizare date locale cu datele din argumentul thread-ului
	thread_arg *threadArg = (thread_arg *)arg;
	int startRow = threadArg->startRow;
	int endRow = threadArg->endRow;
	int n = threadArg->n;
	int m = threadArg->m;
	bool *board = threadArg->board;
	bool *resBoard = threadArg->resBoard;

	for (int i = startRow; i < endRow; i++) {
		for (int j = 0; j < m; j++) {
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
		}
	}
	return NULL;
}

void gameOfLife(bool *board, int n, int m, int numThreads) {
	bool *resBoard = (bool *)malloc(n * m * sizeof(bool));

	pthread_t threads[numThreads];
	thread_arg threadArgs[numThreads];

	int rowsPerThread = n / numThreads;

	// Creare thread-uri
	for (int i = 0; i < numThreads; i++) {
		threadArgs[i].startRow = i * rowsPerThread;
		if (i == numThreads - 1)
			threadArgs[i].endRow = n; 
		else 
			threadArgs[i].endRow = (i + 1) * rowsPerThread;
		threadArgs[i].n = n;
		threadArgs[i].m = m;
		threadArgs[i].board = board;
		threadArgs[i].resBoard = resBoard;

		pthread_create(&threads[i], NULL, gameOfLifeThread, (void *)&threadArgs[i]);
	}

	// Join thread-uri
	for (int i = 0; i < numThreads; i++)
		pthread_join(threads[i], NULL);

    memcpy(board,resBoard,(m*n)*sizeof(bool));
    free(resBoard);
}

int main(int argc, char *argv[]) {
	
	if (argc < 3) {
		printf("Wrong number of arguments. The use of the program is ./game_of_life <input_file> <numar_threaduri>\n");
		return 0;
	}

	// Initializare fisiere de intrare/iesire
	char fileNameIn[100];
	char fileNameOut[100];
	int numThreads = atoi(argv[2]);
	sprintf(fileNameIn, "../../input/%s", argv[1]);
	sprintf(fileNameOut, "../../output/pthreads/result_of_%s", argv[1]);

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

	// Paralelizare utilizand pthreads
	gameOfLife(board, n, m, numThreads);

	// Salvare date in fisierul de iesire
	print_output(board,n,m,outputFile);

	// Eliberare memorie
	free(board);

	return 0;
}
