#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 2

int main(int argc, char *argv[]) {
	
    if (argc < 3) {
		printf("Wrong number of arguments. The use of the program is ./generate <n> <m>");
		return 0;
	}

    // Ranomizare seed pe baza timpului curent
    srand(time(NULL));

	char fileNameOut[100];
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
	sprintf(fileNameOut, "../../input/input_%d_x_%d.txt", n, m);

	FILE *outputFile = fopen(fileNameOut, "w");

	if (outputFile == NULL) {
		perror("Error opening file");
		return 1;
	}

    fprintf(outputFile, "%d %d\n", n,m);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
            fprintf(outputFile, "%d ", rand()%SIZE);
		fprintf(outputFile, "\n");
	}

	fclose(outputFile);
	
	return 0;
}
