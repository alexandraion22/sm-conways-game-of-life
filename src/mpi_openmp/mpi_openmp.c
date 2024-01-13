#include "../utils/conway_utils.h"
#include <mpi.h>
#include <omp.h>
#define MASTER 0

void gameOfLife(bool *board, int n, int m)
{
  	bool *resBoard = (bool *)malloc(n * m * sizeof(bool));
    int i,j;

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

    unsigned int n, m, rank, size, start, end, localN, chunkSize;
    bool *board;
    FILE *inputFile, *outputFile;

	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == MASTER)
    {
        if (argc < 2) {
            printf("Wrong number of arguments. The use of the program is ./game_of_life <input_file>\n");
            return 0;
        }

        // Initializare fisiere de intrare/iesire
        char fileNameIn[100];
        char fileNameOut[100];
        sprintf(fileNameIn, "../../input/%s", argv[1]);
        sprintf(fileNameOut, "../../output/mpi_openmp/result_of_%s", argv[1]);

        inputFile = fopen(fileNameIn, "r");
        outputFile = fopen(fileNameOut, "w");

        if (inputFile == NULL || outputFile == NULL) {
            perror("Error opening file");
            return 1;
        }

        // Alocare spatiu matrice de intrare
        fscanf(inputFile, "%d %d", &n, &m);

        board = (bool *)malloc(n * m * sizeof(bool));

        // Citire date matrice de intrare
        read_input(board,n,m,inputFile);
    }

    // Broadcast dimensiuni matrice
    MPI_Bcast(&n, 1, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);

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
                        MPI_Send(&board[j*m], chunkSize, MPI_C_BOOL, i, 0, MPI_COMM_WORLD);
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

            board = (bool *)malloc(localN * m * sizeof(bool));
        
            for(int i = 0; i < localN; i++)
			    MPI_Recv(&board[i*m], chunkSize, MPI_C_BOOL, MASTER, 0, MPI_COMM_WORLD, NULL);
		}

    // Procesare date
	gameOfLife(board, localN, m);

    // Primire rezultate procesare de la restul proceselor
    if(rank == MASTER)
        for (int i = 1; i < size; i++)
            {
                start = i * (double)n/size;
                end = min(n,(i+1) * (double)n/size);
                    
                for(int j = start; j< end; j++)
                    MPI_Recv(&board[j*m], chunkSize, MPI_C_BOOL, i, 0, MPI_COMM_WORLD, NULL);
			}
    else
    {
        // Calcul start si end pentru portiunea calculata - fara padding
        int start_send = rank * (double)n/size;
        end = min(n,(rank+1) * (double)n/size);
                    
        for(int j = start_send - start; j < end - start; j++)
            MPI_Send(&board[j*m], chunkSize, MPI_C_BOOL, MASTER, 0, MPI_COMM_WORLD);

        // Eliberare memorie
        free(board);
    }

    if (rank == MASTER) {
        // Salvare date in fisierul de iesire
        print_output(board,n,m,outputFile);
        
        // Eliberare memorie
        free(board);
    }

    MPI_Finalize();
	return 0;
}
