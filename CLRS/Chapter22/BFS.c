#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "linkedList.h"
#include "queue.h"

#include "graph.h"

void BFS(Graph *G, unsigned int start) {
	int i;
	for (i = 0; i < G->totalV; i++){
		G->V[i].discovery = INT_MAX;
		G->V[i].parent = -1;
		G->V[i].color = 0;
	}

	G->V[start].discovery = 0;
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
			if (G->V[v].color == 0) {
				G->V[v].color = 1;
				G->V[v].discovery = G->V[u].discovery + 1;
				G->V[v].parent = u;
				enqueue(q, v);
			}
			curr = curr->next;
		}
		G->V[u].color = 1;

		if (q->size == 0) {
			int found = 0;
			int j;
			for (j = 0; j < G->totalV && found == 0; j++) {
				if (G->V[j].color == 0) {
					connectedComponents += 1;
					G->V[j].discovery = 0;	
					enqueue(q, j);
					found = 1;
				}
			}
		}
	}


	fprintf(stdout, "connected components = %d\n", connectedComponents);

	destroyQueue(q);
	return;
}

int main(int argc, char **argv) 
{
	fprintf(stdout, "%d\n", argc);

	if (argc != 3 && argc != 5) { fprintf(stderr, "Invalid Arguments\n");
	return EXIT_FAILURE; }

	char *filename = argv[1];

	int directed = atoi(argv[2]);

	Graph *G = buildGraph(filename, directed);

	if (G == NULL) {
		return EXIT_FAILURE;
	}

	printAdjacencyList(G);

	BFS(G, 0);

	printGraph1(G);

	if (argc == 5) {
		int start = atoi(argv[3]);
		int end = atoi(argv[4]);
		printPath(G, start, end);	
	}

	destroyGraph(G);
	
	return EXIT_SUCCESS;
}