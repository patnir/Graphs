#include <stdio.h>
#include <stdlib.h>

#include "linkedList.h"

typedef struct _Vertex {
	LinkedList *adjacent;
	int x, y;
	unsigned int value;
	unsigned int distance;
	unsigned int parent;
	short visited;
} Vertex;

typedef struct _Graph {
	Vertex *V;
	unsigned int totalV;
	unsigned int totalE;
} Graph;

void destroyGraph(Graph *G) {
	int i;
	fprintf(stdout, "Destroying Graph\n");
	for (i = 0; i < G->totalV; i++) {
		destroyLinkedList(G->V[i].adjacent);
	}
	free(G->V);
	free(G);
}

void printGraph(Graph *G) {
	int i;
	fprintf(stdout, "Printing Graph\n");
	for(i = 0; i < G->totalV; i++) {
		fprintf(stdout, "%d: ", i);
		printLinkedList(G->V[i].adjacent);
	}
}

Graph *buildGraph(char *filename) {
	Graph *G = (Graph *)malloc(sizeof(Graph));
	G->V = NULL;

	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error opening file\n");
		free(G);
		return NULL;
	}

	int V;
	int E;

	fscanf(fptr, "%d\n", &V);
	fscanf(fptr, "%d\n", &E);

	G->totalV = V;
	G->totalE = E;

	G->V = (Vertex *)malloc(sizeof(Vertex) * V);

	int i;

	for (i = 0; i < V; i++) {
		G->V[i].adjacent = createLinkedList();
	}

	for (i = 0; i < E; i++) {
		int v;
		int e;
		fscanf(fptr, "%d ", &v);
		fscanf(fptr, "%d\n", &e);
		pushNode(G->V[v].adjacent, e);
	}

	fclose(fptr);

	return G;
}

int main(int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "Invalid Arguments\n");
		return EXIT_FAILURE;
	}

	char *filename = argv[1];

	Graph *G = buildGraph(filename);

	if (G == NULL) {
		return EXIT_FAILURE;
	}

	printGraph(G);
	destroyGraph(G);

	return EXIT_SUCCESS;
}