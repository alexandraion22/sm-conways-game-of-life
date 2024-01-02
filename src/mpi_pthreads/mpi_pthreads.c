#include "../utils/conway_utils.h"
#include <mpi.h>
#include <pthread.h>
#define MASTER 0

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

    // Salvare rezultat
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

    unsigned int n, m, rank, size, start, end, localN, chunkSize, numThreads;
    bool **board;
    FILE *inputFile, *outputFile;

	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == MASTER)
    {
        if (argc < 3) {
            printf("Wrong number of arguments. The use of the program is ./game_of_life <input_file> <numar_threaduri>\n");
            return 0;
        }

        numThreads = atoi(argv[2]);

        // Initializare fisiere de intrare/iesire
        char fileNameIn[100];
        char fileNameOut[100];
        sprintf(fileNameIn, "../../input/%s", argv[1]);
        sprintf(fileNameOut, "../../output/mpi_pthreads/result_of_%s", argv[1]);

        inputFile = fopen(fileNameIn, "r");
        outputFile = fopen(fileNameOut, "w");

        if (inputFile == NULL || outputFile == NULL) {
            perror("Error opening file");
            return 1;
        }

        // Alocare spatiu matrice de intrare
        fscanf(inputFile, "%d %d", &n, &m);

        board = (bool **)malloc(n * sizeof(bool *));
        for (int i = 0; i < n; i++)
            board[i] = (bool *)malloc(m * sizeof(bool));

        // Citire date matrice de intrare
        read_input(board,n,m,inputFile);
    }

    // Broadcast dimensiuni matrice
    MPI_Bcast(&n, 1, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(&numThreads, 1, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);

    chunkSize = sizeof(bool) * m;
    
    // Trimitere bucati din matricea initiala de la MASTER catre celelalte procese
    if(rank == MASTER)
		{
			for (int i = 1; i < size; i++)
            	{
                    // Calcul start si end cu padding
                    start = max(0,i * (double)n/size - 1);
                    end = min(n,(i+1) * (double)n/size + 1);
                    
                    // Trimitere linii corespunzatoare fiecarui proces (+ linii pe o parte si cealalta, daca e posibil)
                    for(int j = start; j< end; j++)
                        MPI_Send(board[j], chunkSize, MPI_C_BOOL, i, 0, MPI_COMM_WORLD);
				}
            
            // Calcul localN - cat din matrice proceseaza procesul 0
            localN = min(n,(double)n / size + 1);
		}
	else
		{
            // Calcul start si end cu padding
            start = max(0,rank * (double)n/size - 1);
            end = min(n,(rank+1) * (double)n/size + 1);
            
            localN = end - start;

            board = (bool **)malloc(localN * sizeof(bool *));
            for (int i = 0; i < localN; i++)
                board[i] = (bool *)malloc(m * sizeof(bool));
        
            for(int i = 0; i < localN; i++)
			    MPI_Recv(board[i], chunkSize, MPI_C_BOOL, MASTER, 0, MPI_COMM_WORLD, NULL);
		}

    // Procesare date
	gameOfLife(board, localN, m, numThreads);

    // Primire rezultate procesare de la restul proceselor
    if(rank == MASTER)
        for (int i = 1; i < size; i++)
            {
                start = i * (double)n/size;
                end = min(n,(i+1) * (double)n/size);
                    
                for(int j = start; j< end; j++)
                    MPI_Recv(board[j], chunkSize, MPI_C_BOOL, i, 0, MPI_COMM_WORLD, NULL);
			}
    else
    {
        // Calcul start si end pentru portiunea calculata - fara padding
        int start_send = rank * (double)n/size;
        end = min(n,(rank+1) * (double)n/size);
                    
        for(int j = start_send - start; j < end - start; j++)
            MPI_Send(board[j], chunkSize, MPI_C_BOOL, MASTER, 0, MPI_COMM_WORLD);

        // Eliberare memorie
        for (int i = 0; i < localN; i++)
            free(board[i]);
        free(board);
    }

    if (rank == MASTER) {
        // Salvare date in fisierul de iesire
        print_output(board,n,m,outputFile);
        
        // Eliberare memorie
        for (int i = 0; i < n; i++)
            free(board[i]);
        free(board);
    }

    MPI_Finalize();
	return 0;
}
