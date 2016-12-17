#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "quickSort.c"

typedef struct {
	int x; int y; int index;
	int adjacentCount;
	int* adjacentPoints;
}Vertex;

typedef struct RNode{
	int vertex;
	struct Rnode *next;
}Rnode;

void sortPointsList(Vertex *vertices, int totalV);
void freeVertices(Vertex *vertices, int totalV);

Vertex *buildAdjacencyMatrix(char *filename, int *totalV, int *totalE);
Vertex *buildSpaceEfficientAdjacencyMatrix(char *filename, int *totalV, int *totalE);
Vertex *buildAdjacencyList(char *filename, int *totalV, int *totalE);

void printVertices(Vertex *vertices, int totalV);
int distance(int x1, int y1, int x2, int y2);


void minCandidateForLoop(Vertex *vertices, unsigned int *minDistance, int *minPoint, int *distances, int *X, int length, int *parentIndex, int *destinationFound, int end) 
{
	int q;
	for (q = 0; q < length; q++) {
		int j;
		int i = X[q]; // current vertex from side X being evaluated
		for (j = 0; j < vertices[i].adjacentCount; j++) {
			int candidate = vertices[i].adjacentPoints[j]; // candidate vertext from side V - X
			fprintf(stdout, "considering candidate %d\n", candidate);
			int k;
			int notACandidate = 0;
			for (k = 0; k < length; k++) {
				if (candidate == X[k]) {
					notACandidate = 1;
					break;
				}
			}
			if (notACandidate == 0) {
				if (candidate == end) {
					*destinationFound = 1;
				}
				int candidateX = vertices[candidate].x;
				int candidateY = vertices[candidate].y;
				int currDistance = distance(vertices[i].x, vertices[i].y, candidateX, candidateY);
				currDistance += distances[i];
				fprintf(stdout, "current distance = %d\n", currDistance);
				if (currDistance < *minDistance) {
					*parentIndex = i;
					*minDistance = currDistance;
					*minPoint = candidate;
				}
			}
		}
	}
}

void printingShortestRoute(int *parents, int start, int end, int totalV) 
{
	int *route = (int *)malloc(sizeof(int) * totalV);
	int index = 0;
	int curr = end;

	while (curr != start) {
		//fprintf(stdout, "%d ", curr);
		route[index++] = curr;
		curr = parents[curr];
	}

	route[index] = start;

	fprintf(stdout, "printing route: ");
	while (index >= 0) {
		fprintf(stdout, "%d ", route[index--]);
	}
	fprintf(stdout, "\n\n\n");

	free(route);
}

int shortestPath(Vertex *vertices, int start, int end, int totalV, int totalE) 
{
	if (start == end) {
		return 0;
	}

	int length = 0;
	int *X = (int *)malloc(sizeof(int) * totalV);
	int *distances = (int *)malloc(sizeof(int) * totalV);
	int *parents = (int *)malloc(sizeof(int) * totalV);
	//Rnode *route = (Rnode *)malloc(sizeof(Rnode) * totalV);

	int destinationFound = 0;

	X[length++] = start;
	distances[start] = 0;

	while (length != totalV) {
		unsigned int minDistance = INT_MAX;
		int minPoint = -1;
		int parentIndex = -1;

		minCandidateForLoop(vertices, &minDistance, &minPoint, distances, X, length, &parentIndex, &destinationFound, end);
		
		if (minPoint == -1 && destinationFound == 0) {
			fprintf(stdout, "Error finding minimum on border, there is probably a closed cycle\n");
			free(X);
			free(distances);
			return -1;
		}
		else {
			if (minPoint != -1) {
				fprintf(stdout, "Length = %d, minPoint = %d, minDistance = %d, parent = %d\n", length, minPoint, minDistance, parentIndex);
				X[length++] = minPoint;
				distances[minPoint] = minDistance;
				parents[minPoint] = parentIndex;
			}
			else {
				length = totalV;
			}
		}
	}

	fprintf(stdout, "\n\ndistance of destination %d is %d\n", end, distances[end]);

	printingShortestRoute(parents, start, end, totalV); 

	free(X);
	free(distances);
	free(parents);
	return distances[end];
}

void relax(Vertex *vertices, int u, int v, int unsigned distanceU, unsigned int *distances, int *parents, unsigned int *determinedDistances) 
{
	int distanceToAdd = distance(vertices[u].x, vertices[u].y, vertices[v].x, vertices[v].y);
	if (determinedDistances[v] > distanceU + distanceToAdd && distances[v] > distanceU + distanceToAdd) {
		distances[v] = distanceU + distanceToAdd;
		parents[v] = u;
	}
}

void initializeSingleSource(Vertex *vertices, int totalV, int start, unsigned int *distances, int *parents, unsigned int *determinedDistances) 
{
	int i;
	for (i = 0; i < totalV; i++) {
		distances[i] = INT_MAX;
		determinedDistances[i] = INT_MAX;
		parents[i] = -1;
	}

	distances[start] = INT_MAX;

	for (i = 0; i < vertices[start].adjacentCount; i++) {
		int v = vertices[start].adjacentPoints[i];
		relax(vertices, start, v, 0, distances, parents, determinedDistances);
	}
}

int extractMinPoint(unsigned int *distances, int totalV) 
{
	int minPoint = -1;
	unsigned int minDistance = INT_MAX;
	int i;
	for (i = 0; i < totalV; i++) {
		if (minDistance > distances[i]) {
			minDistance = distances[i];
			minPoint = i;
		}
	}

	return minPoint;
}

void printDistances(unsigned int *distances, int totalV) 
{
	int i;
	for (i = 0; i < totalV; i++) {
		fprintf(stdout, "%d ", distances[i]);
	}
	fprintf(stdout, "\n");
}

unsigned int Dijkstra(Vertex *vertices, int start, int end, int totalV, int totalE)
{
	unsigned int *distances = (unsigned int *)malloc(sizeof(unsigned int) * totalV);
	unsigned int *determinedDistances = (unsigned int *)malloc(sizeof(unsigned int) * totalV);

	int *parents = (int *)malloc(sizeof(int) * totalV);
	initializeSingleSource(vertices, totalV, start, distances, parents, determinedDistances);

	printDistances(distances, totalV);

	int length = 1;

	determinedDistances[start] = 0;

	while (length != totalV) {
		int u = extractMinPoint(distances, totalV);

		determinedDistances[u] = distances[u];
		distances[u] = INT_MAX;

		int i;
		for (i = 0; i < vertices[u].adjacentCount; i++) {
			int v = vertices[u].adjacentPoints[i];
			relax(vertices, u, v, determinedDistances[u], distances, parents, determinedDistances);
		}

		length += 1;
	}

	int i = 0;
	for (i = 0; i < totalV; i++) {
		if (distances[i] < determinedDistances[i]) {
			determinedDistances[i] = distances[i];
		}
	}

	fprintf(stdout, "printing determiedn distances\n");
	printDistances(determinedDistances, totalV);

	fprintf(stdout, "total distance from %d to %d is %d\n", start, end, determinedDistances[end]);
	unsigned int sd = determinedDistances[end];

	printingShortestRoute(parents, start, end, totalV); 

	free(determinedDistances);
	free(distances);
	free(parents);

	return sd;
}

void readQueries(Vertex *vertices, int totalV, int totalE, char *filename) 
{
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error opening queries file\n");
		return;
	}

	int totalQ;

	fscanf(fptr, "%d", &totalQ);

	fprintf(stdout, "total number of queries: %d\n", totalQ);

	int i;

	for (i = 0; i < totalQ; i++) {
		int start;
		int end;

		fscanf(fptr, "%d", &start);
		fscanf(fptr, "%d", &end);

		fprintf(stdout, "***************************************start = %d and end = %d\n", start, end);

		unsigned int shortestDistance = Dijkstra(vertices, start, end, totalV, totalE);

		if (shortestDistance == -1) {
			fprintf(stdout, "!!!!!!!!!!!!!!!!!!Error, -1 returned on finding shortest path\n\n");
			//fclose(fptr);
			//return;
		}
		else {
			fprintf(stdout, "The shortest distance is %d units\n", shortestDistance);
		}
	}

	fclose(fptr);

	return;
}

int main(int argc, char **argv) 
{
	if (argc != 3) {
		fprintf(stderr, "Invalid number of arguments\n");
		return EXIT_FAILURE;
	}
	int totalV;
	int totalE;
	Vertex *vertices = buildSpaceEfficientAdjacencyMatrix(argv[1], &totalV, &totalE);
	if (vertices == NULL) {
		return EXIT_FAILURE;
	}

	//sortPointsList(vertices, totalV);
	printVertices(vertices, totalV);

	readQueries(vertices, totalV, totalE, argv[2]);

	freeVertices(vertices, totalV);

	return EXIT_SUCCESS;
}

int distance(int x1, int y1, int x2, int y2)
{
	//fprintf(stdout, "calculating distances %d, %d, %d, %d\n", x1, y1, x2, y2);
	return (int) sqrt(pow((double)(x1 - x2), 2.0) + pow((double)(y1 - y2), 2.0));
}

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

Vertex *buildAdjacencyList(char *filename, int *totalV, int *totalE) {
	FILE *fptr = fopen(filename, "r");

	if (fptr == NULL) {
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	fscanf(fptr, "%d ", totalV);
	fscanf(fptr, "%d ", totalE);

	Vertex *vertices = (Vertex *)malloc(sizeof(Vertex) * (*totalV));
	free(vertices);

	return NULL;
}

Vertex *buildAdjacencyMatrix(char *filename, int *totalV, int *totalE) 
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
		vertices[i].adjacentPoints = (int *)malloc(sizeof(int) * (*totalV));
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

Vertex *buildSpaceEfficientAdjacencyMatrix(char *filename, int *totalV, int *totalE) 
{
	fprintf(stdout, "doing this space efficiently!!\n");

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
	}


	for (i = 0; i < *totalE; i++) {
		int curr;
		int point;
		fscanf(fptr, "%d ", &curr);
		fscanf(fptr, "%d ", &point);
		vertices[curr].adjacentCount++;
		vertices[point].adjacentCount++;
	}

	for (i = 0; i < *totalV; i++) {
		vertices[i].adjacentPoints = (int *)malloc(sizeof(int) * (vertices[i].adjacentCount));
	}

	fseek(fptr, 0, SEEK_SET);

	fscanf(fptr, "%d ", totalV);
	fscanf(fptr, "%d ", totalE);

	for (i = 0; i < *totalV; i++) {
		vertices[i].adjacentCount = 0;
		fscanf(fptr, "%d ", &((vertices[i]).index));
		fscanf(fptr, "%d ", &((vertices[i]).x));
		fscanf(fptr, "%d ", &((vertices[i]).y));
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


/*void shortestPath(Vertex* graph, int totalV, int totalE, int source) 
{
	SP *U = (SP *)malloc(sizeof(SP) * totalV);
	int i;
	for (i = 0; i < totalV; i++) {
		U[i].vertex = -1;
		U[i].minLength = INT_MAX;
		U[i].visited = 0;
	}
	U[source].minLength = 0;
	U[source].visited = 1;

	SP *V = (SP *)malloc(sizeof(SP) * totalV);

	int total = 1;

	int minDest = 0;
	int minLength = INT_MAX;

	while (total <= totalV) {
		for (i = 0; i < totalV; i++) {
			if (U[i].visited == 1) {
				int j;
				for (j = 0; j < adjacentCount; j++) {	
				}
			}
		}
		total += 1;
	}

	free(U);
	free(V);
}*/