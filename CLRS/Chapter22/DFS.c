#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "linkedList.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct _Vertex {
	LinkedList *adjacent;
	int x, y;
	unsigned int discovery;
	unsigned int finishing;
	int parent;
	short color;
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

void printAdjacencyList(Graph *G) {
	int i;
	fprintf(stdout, "Printing Adjacency List\n");
	for(i = 0; i < G->totalV; i++) {
		fprintf(stdout, "%d: ", i);
		printLinkedList(G->V[i].adjacent);
	}
}

void printGraph(Graph *G) {
	int i;
	fprintf(stdout, "Printing Graph\n");
	for(i = 0; i < G->totalV; i++) {
		fprintf(stdout, "%d: d = %d, f = %d, p = %d, v = %d\n", i, G->V[i].discovery, G->V[i].finishing, G->V[i].parent, G->V[i].color);
	}
}

Graph *buildGraph(char *filename, int directed) {
	if (directed == 0) {
		fprintf(stdout, "Building undirected graph\n");
	}
	else {
		fprintf(stdout, "Building directed graph\n");
	}

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
		G->V[i].x = 0;
		G->V[i].y = 0;
	}

	for (i = 0; i < E; i++) {
		int v;
		int e;
		fscanf(fptr, "%d ", &v);
		fscanf(fptr, "%d\n", &e);
		pushNode(G->V[v].adjacent, e);
		if (directed == 0) {
			pushNode(G->V[e].adjacent, v);
		}
	}

	fclose(fptr);

	return G;
}

void printPath(Graph *G, int start, int end) {
	fprintf(stdout, "Printing path:\n");
	if (end == start) {
		fprintf(stdout, "%d\n", start);
		return;
	}

	int *path = (int *)calloc(G->totalV, sizeof(int));

	int i = 0;

	int curr = end;

	while (curr != start && curr != -1 && i < G->totalV) {
		path[i++] = curr;
		curr = G->V[curr].parent;
	}

	if (curr == -1) {
		fprintf(stdout, "Path not found\n");
	} 
	else {
		int j;
		fprintf(stdout, "%d-", curr);
		for (j = i - 1; j > 0; j--) {
			fprintf(stdout, "%d-", path[j]);
		}
		fprintf(stdout, "%d\n", path[0]);
		
	}

	free(path);
}

int time;

void DFS_Visit(Graph *G, int u) {
	G->V[u].discovery = ++time;

	G->V[u].color = GRAY;

	Node *curr;

	for(curr = G->V[u].adjacent->head; curr != NULL; curr = curr->next) {
		int v = curr->value;
		if (G->V[v].color == WHITE) {
			G->V[v].parent = u;
			//fprintf(stdout, "does this never happend\n");
			DFS_Visit(G, v);
		}
	}

	G->V[u].color = BLACK;
	G->V[u].finishing = ++time;
}

void DFS(Graph *G) {
	int i;
	for (i = 0; i < G->totalV; i++){
		G->V[i].parent = -1;
		G->V[i].color = WHITE;
	}

	time = 0;

	for (i = 0; i < G->totalV; i++) {
		if (G->V[i].color == WHITE) {
			DFS_Visit(G, i);
		}
	}
}

int main(int argc, char **argv) {

	if (argc != 3 && argc != 5) { 
		fprintf(stderr, "Invalid Arguments\n");
		return EXIT_FAILURE; 
	}

	char *filename = argv[1];

	int directed = atoi(argv[2]);

	Graph *G = buildGraph(filename, directed);

	if (G == NULL) {
		return EXIT_FAILURE;
	}

	printAdjacencyList(G);

	DFS(G);

	printGraph(G);

	if (argc == 5) {
		int start = atoi(argv[3]);
		int end = atoi(argv[4]);
		printPath(G, start, end);	
	}

	destroyGraph(G);	
	
	return EXIT_SUCCESS;
}