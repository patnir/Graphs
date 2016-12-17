#include<stdio.h>
#include<stdlib.h>
#include "quickSort.c"

typedef struct {
	int x; int y; int index;
	int adjacentCount;
	int* adjacentPoints;
}Vertex;

void printVertices(Vertex *vertices, int totalV) 
{
	int i;
	//for (i = 0; i < totalV; i++) {
	//	fprintf(stdout, "vertex #%d: x = %d, y = %d, adjacent points count = %d\n", vertices[i].index, vertices[i].x, vertices[i].y, vertices[i].adjacentCount);
	//}
	for (i = 0; i < totalV; i++) {
		fprintf(stdout, "%d: ", vertices[i].index);
		int j;
		for (j = 0; j < vertices[i].adjacentCount; j++) {
			fprintf(stdout, "%d ", vertices[i].adjacentPoints[j]);
		}
		fprintf(stdout, "\n");
	}
}

Vertex *buildAdjacencyList(char *filename, int *totalV, int *totalE) 
{
	FILE *fptr = fopen(filename, "r");

	if (fptr == NULL) {
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	fscanf(fptr, "%d ", totalV);
	fscanf(fptr, "%d ", totalE);

	//fprintf(stdout, "number of vertices = %d\nnumber of edges = %d\n", *totalV, *totalE); 

	Vertex *vertices = (Vertex *)malloc(sizeof(Vertex) * (*totalV));

	int i;

	for (i = 0; i < *totalV; i++) {
		vertices[i].adjacentCount = 0;
		fscanf(fptr, "%d ", &((vertices[i]).index));
		fscanf(fptr, "%d ", &((vertices[i]).x));
		fscanf(fptr, "%d ", &((vertices[i]).y));
		vertices[i].adjacentPoints = (int *)malloc(sizeof(int) * (*totalE));
	}


	for (i = 0; i < *totalE; i++) {
		int curr;
		int point;
		fscanf(fptr, "%d ", &curr);
		fscanf(fptr, "%d ", &point);
		vertices[curr].adjacentPoints[vertices[curr].adjacentCount++] = point;
		vertices[point].adjacentPoints[vertices[point].adjacentCount++] = curr;
	}

	fclose(fptr);
	return vertices;
}

void freeVertices(Vertex *vertices, int totalV) 
{
	int i = 0;
	for (i = 0; i < totalV; i++) {
		free(vertices[i].adjacentPoints);
	}	
	free(vertices);
}

void sortPointsList(Vertex *vertices, int totalV)
{
	int i;
	for (i = 0; i < totalV; i++) {
		quickSort(vertices[i].adjacentPoints, 0, vertices[i].adjacentCount - 1);	
	}
}

int main(int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "Invalid number of arguments\n");
		return EXIT_FAILURE;
	}
	int totalV;
	int totalE;
	Vertex *vertices = buildAdjacencyList(argv[1], &totalV, &totalE);
	if (vertices == NULL) {
		return EXIT_FAILURE;
	}

	//sortPointsList(vertices, totalV);
	printVertices(vertices, totalV);

	freeVertices(vertices, totalV);
	return EXIT_SUCCESS;
}
