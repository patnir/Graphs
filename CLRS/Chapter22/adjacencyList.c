#include <stdio.h>
#include <stdlib.h>

#include "linkedList.h"

typedef struct AdjacencyList {
	int size;
	LinkedList **lists;
} AdjacencyList;

void destroyAdjacencyList(AdjacencyList *aList) {
	int i;
	fprintf(stdout, "Destroying Adjacency List\n");
	for (i = 0; i < aList->size; i++) {
		destroyLinkedList(aList->lists[i]);
	}
	free(aList->lists);
	free(aList);
}

void printAdjacencyList(AdjacencyList *aList) {
	int i;
	fprintf(stdout, "Printing Adjacency List\n");
	for (i = 0; i < aList->size; i++) {
		printLinkedList(aList->lists[i]);
	}
}

AdjacencyList *buildAdjacencyList(char *filename) {
	AdjacencyList *aList = (AdjacencyList *)malloc(sizeof(AdjacencyList));
	aList->lists = NULL;

	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	int V;
	int E;

	fscanf(fptr, "%d\n", &V);
	fscanf(fptr, "%d\n", &E);

	aList->size = V;

	int i;

	aList->lists = (LinkedList **)malloc(sizeof(LinkedList *) * V);

	for (i = 0; i < V; i++) {
		aList->lists[i] = createLinkedList();
	}

	for (i = 0; i < E; i++) {
		int v;
		int e;
		fscanf(fptr, "%d ", &v);
		fscanf(fptr, "%d\n", &e);

		pushNode(aList->lists[v], e);
	}

	fprintf(stdout, "total vertices and edges = %d, %d\n", V, E);
	fclose(fptr);

	return aList;
}

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

	AdjacencyList *aList = buildAdjacencyList(filename);
	
	printAdjacencyList(aList);
	destroyAdjacencyList(aList);

	return EXIT_SUCCESS;
}