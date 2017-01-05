#include <stdio.h>
#include <stdlib.h>

typedef struct _AdjacencyMatrix {
	unsigned int **matrix;
	int V;
	int E;
} AdjacencyMatrix;

void destroyAdjacencyMatrix(AdjacencyMatrix *mat) 
{
	fprintf(stdout, "Destroying Adjacency Matrix\n");
	int i;
	for (i = 0; i < mat->V; i++) {
		free(mat->matrix[i]);
	}
	free(mat->matrix);
	free(mat);
}

void printAdjacencyMatrix(AdjacencyMatrix *mat) 
{
	fprintf(stdout, "Printing Adjacency Matrix\n");
	int i;
	int j;
	fprintf(stdout, "    ");
	for (i = 0; i < mat->V; i++) {
		fprintf(stdout, "%4d", i);
	}
	for (i = 0; i < mat->V; i++) {
		fprintf(stdout,"\n%3d: ", i);
		for (j = 0; j < mat->V; j++) {
			fprintf(stdout, "%3d ", mat->matrix[i][j]);
		}
	}
	fprintf(stdout, "\n");
}

AdjacencyMatrix *buildAdjacencyMatrix(char *filename) 
{
	AdjacencyMatrix *mat = (AdjacencyMatrix *)malloc(sizeof(AdjacencyMatrix));

	if (mat == NULL) {
		fprintf(stderr, "Error allocation space for adMatrix\n");
		return NULL;
	}

	mat->matrix = NULL;

	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error opening file\n");
		free(mat);
		return NULL;
	}

	int V;
	int E;

	fscanf(fptr, "%d\n", &V);
	fscanf(fptr, "%d\n", &E);

	mat->V = V;
	mat->E = E;

	int i;

	mat->matrix = (unsigned int **)malloc(sizeof(unsigned int *) * V);

	for (i = 0; i < V; i++) {
		mat->matrix[i] = (unsigned int *)calloc(V, sizeof(unsigned int));
	}
	for (i = 0; i < E; i++) {
		unsigned int v;
		unsigned int e;
		fscanf(fptr, "%d ", &v);
		fscanf(fptr, "%d\n", &e);
		mat->matrix[v][e] = 1;
	}

	fclose(fptr);

	return mat;
}

int main(int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "Invalid Arguments\n");
		return EXIT_FAILURE;
	}

	char *filename = argv[1];

	AdjacencyMatrix *mat = buildAdjacencyMatrix(filename);
	printAdjacencyMatrix(mat);
	destroyAdjacencyMatrix(mat);
	return EXIT_SUCCESS;
}