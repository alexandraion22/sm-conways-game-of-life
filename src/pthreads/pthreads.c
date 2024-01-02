#include "../utils/conway_utils.h"
#include <pthread.h>

typedef struct {
    int startRow;
    int endRow;
    int n;
    int m;
    bool **board;
    bool **resBoard;
    pthread_barrier_t *barrier;
} thread_arg;

void *gameOfLifeThread(void *arg) {

	// Initalizare date locale cu datele din argumentul thread-ului
	thread_arg *threadArg = (thread_arg *)arg;
	int startRow = threadArg->startRow;
	int endRow = threadArg->endRow;
	int n = threadArg->n;
	int m = threadArg->m;
	bool **board = threadArg->board;
	bool **resBoard = threadArg->resBoard;

	for (int i = startRow; i < endRow; i++) {
		for (int j = 0; j < m; j++) {
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
		}
	}

    // Bariera
    pthread_barrier_wait(threadArg->barrier);

    // Eliberare memorie si salvare rezultat
	for (int i = startRow; i < endRow; i++) {
		for (int j = 0; j < m; j++) 
			board[i][j] = resBoard[i][j];
	}
	return NULL;
}

void gameOfLife(bool **board, int n, int m, int numThreads) {
	bool **resBoard = (bool **)malloc(n * sizeof(bool *));
	for (int i = 0; i < n; i++)
		resBoard[i] = (bool *)malloc(m * sizeof(bool));

	pthread_t threads[numThreads];
	thread_arg threadArgs[numThreads];
    
    // Initializare bariera
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier,NULL,numThreads);

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
        threadArgs[i].barrier = &barrier;

		pthread_create(&threads[i], NULL, gameOfLifeThread, (void *)&threadArgs[i]);
	}

	// Join thread-uri
	for (int i = 0; i < numThreads; i++)
		pthread_join(threads[i], NULL);

    pthread_barrier_destroy(&barrier);
	
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

	bool **board = (bool **)malloc(n * sizeof(bool *));
	for (int i = 0; i < n; i++)
		board[i] = (bool *)malloc(m * sizeof(bool));

	// Citire date matrice de intrare
	read_input(board,n,m,inputFile);

	// Paralelizare utilizand pthreads
	gameOfLife(board, n, m, numThreads);

	// Salvare date in fisierul de iesire
	print_output(board,n,m,outputFile);

	// Eliberare memorie
	for (int i = 0; i < n; i++)
		free(board[i]);
	free(board);

	return 0;
}
