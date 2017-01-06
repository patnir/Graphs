#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "linkedList.h"
#include "queue.h"


typedef struct _Vertex {
	LinkedList *adjacent;
	int x, y;
	unsigned int distance;
	int parent;
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
		fprintf(stdout, "%d: d = %d, p = %d, v = %d\n", i, G->V[i].distance, G->V[i].parent, G->V[i].visited);
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
		G->V[i].x = 0;
		G->V[i].y = 0;
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

void BFS(Graph *G, unsigned int start) {
	int i;
	for (i = 0; i < G->totalV; i++){
		G->V[i].distance = INT_MAX;
		G->V[i].parent = -1;
		G->V[i].visited = 0;
	}

	G->V[start].distance = 0;
	Queue *q = createQueue();

	printGraph(G);

	enqueue(q, start);

	int connectedComponents = 1;

	while (q->size != 0) {
		printQueue(q);
		int u = dequeue(q);
		Node *curr = G->V[u].adjacent->head;
		while (curr != NULL) {
			unsigned int v = curr->value;
			if (G->V[v].visited == 0) {
				G->V[v].visited = 1;
				G->V[v].distance = G->V[u].distance + 1;
				G->V[v].parent = u;
				enqueue(q, v);
			}
			curr = curr->next;
		}
		G->V[u].visited = 1;

		if (q->size == 0) {
			int found = 0;
			int j;
			for (j = 0; j < G->totalV && found == 0; j++) {
				if (G->V[j].visited == 0) {
					connectedComponents += 1;
					G->V[j].distance = 0;	
					enqueue(q, j);
					found = 1;
				}
			}
		}
	}


	fprintf(stdout, "connected components = %d\n", connectedComponents);

	printGraph(G);

	destroyQueue(q);
	return;
}

void testQueue() {
	Queue *q = createQueue();
	int i;
	for (i = 0; i < 6; i++) {
		enqueue(q, i);
		printQueue(q);
	}

	for (i = 0; i < 6; i++) {
		dequeue(q);
		printQueue(q);
	}

	printQueue(q);
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

	printAdjacencyList(G);

	BFS(G, 0);
	destroyGraph(G);


	//testQueue();
	return EXIT_SUCCESS;
}