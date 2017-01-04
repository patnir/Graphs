#include <stdio.h>
#include <stdlib.h>

void readFile(char *filename) 
{
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error opening file\n");
		return;
	}

	int V;
	int E;

	fscanf(fptr, "%d\n", &V);
	fscanf(fptr, "%d\n", &E);

	fprintf(stdout, "total vertices and edges = %d, %d\n", V, E);
	fclose(fptr);

	return;
}

int main(int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "Invalid Arguments\n");
		return EXIT_FAILURE;
	}

	char *filename = argv[1];

	readFile(filename);

	return EXIT_SUCCESS;
}