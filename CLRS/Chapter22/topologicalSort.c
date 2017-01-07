#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "linkedList.h"
#include "graph.h"

int time;

void DFS_Visit(Graph *G, int u, LinkedList *sortedList) {
	G->V[u].discovery = ++time;

	G->V[u].color = GRAY;

	Node *curr;

	for(curr = G->V[u].adjacent->head; curr != NULL; curr = curr->next) {
		int v = curr->value;
		if (G->V[v].color == WHITE) {
			G->V[v].parent = u;
			//fprintf(stdout, "does this never happend\n");
			DFS_Visit(G, v, sortedList);
		}
	}

	G->V[u].color = BLACK;
	G->V[u].finishing = ++time;
	pushNode(sortedList, u);
}

void DFS(Graph *G, LinkedList *sortedList) {
	int i;
	for (i = 0; i < G->totalV; i++){
		G->V[i].parent = -1;
		G->V[i].color = WHITE;
	}

	time = 0;

	for (i = 0; i < G->totalV; i++) {
		if (G->V[i].color == WHITE) {
			DFS_Visit(G, i, sortedList);
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

	LinkedList *sortedList = createLinkedList();

	DFS(G, sortedList);

	printGraph(G);

	if (argc == 5) {
		int start = atoi(argv[3]);
		int end = atoi(argv[4]);
		printPath(G, start, end);	
	}


	fprintf(stdout, "topological sort: ");
	printLinkedList(sortedList);
	destroyLinkedList(sortedList);
	destroyGraph(G);	
	
	return EXIT_SUCCESS;
}