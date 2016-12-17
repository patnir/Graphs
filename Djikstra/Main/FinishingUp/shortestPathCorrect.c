#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
//#include "quickSort.c"

typedef struct {
	int x; int y; int index;
	int adjacentCount;
	int* adjacentPoints;
}Vertex;

typedef struct {
	unsigned int distance;
	int index;
}Candidate;

void sortPointsList(Vertex *vertices, int totalV);
void freeVertices(Vertex *vertices, int totalV);

Vertex *buildAdjacencyMatrix(char *filename, int *totalV, int *totalE);
Vertex *buildSpaceEfficientAdjacencyMatrix(char *filename, int *totalV, int *totalE);

void printVertices(Vertex *vertices, int totalV);
void readQueries(Vertex *vertices, int totalV, int totalE, char *filename);
int distance(int x11, int y11, int x21, int y21);

void printingShortestRoute(int *route, int *parents, int start, int end, int totalV, unsigned int sd) 
{
	int index = 0;
	int curr = end;

	while (curr != start) {
		//fprintf(stdout, "%d ", curr);
		route[index++] = curr;
		curr = parents[curr];
		if (curr == -1) {
			fprintf(stdout, "INF\n%d %d\n", start, end);
			free(route);
			return;
		}
	}

	fprintf(stdout, "%d\n", sd);

	route[index] = start;

	//fprintf(stdout, "printing route: ");
	while (index >= 0) {
		fprintf(stdout, "%d ", route[index--]);
	}
	fprintf(stdout, "\n");
}

void relax(Vertex *vertices, int u, int v, int unsigned distanceU, unsigned int *distances, int *parents, int *change) 
{
	int distanceToAdd = distance(vertices[u].x, vertices[u].y, vertices[v].x, vertices[v].y);
	if (distances[v] > distanceU + distanceToAdd) {
		distances[v] = distanceU + distanceToAdd;
		parents[v] = u;
		*change = 1;
	}
}

void initializeSingleSource(Vertex *vertices, int totalV, int start, unsigned int *distances, int *parents, Candidate *C, int *totalCandidates)
{
	int i;
	for (i = 0; i < totalV; i++) {
		distances[i] = INT_MAX;
		parents[i] = -1;
	}

	distances[start] = 0;

	int change = 0;

	for (i = 0; i < vertices[start].adjacentCount; i++) {
		int v = vertices[start].adjacentPoints[i];
		relax(vertices, start, v, 0, distances, parents, &change);
		C[*totalCandidates].distance = distances[v];
		C[*totalCandidates].index = v;
		*totalCandidates += 1;
	}
}

void exchangeC(Candidate *C, int *totalCandidates, int index) 
{
	int i = 0;
	while (C[i].index != index) {
		i++;
	}
	index = i;
	*totalCandidates -= 1;
	C[index].index = C[*totalCandidates].index;
	C[index].distance = C[*totalCandidates].distance;
}

int extractMinPoint(unsigned int *distances, Candidate *C, int totalCandidates) 
{
	int i;
	int minPoint = -1;
	unsigned int minDistance = INT_MAX;
	for (i = 0; i < totalCandidates; i++) {
		if (minDistance > C[i].distance) {
			minDistance = C[i].distance;
			minPoint = C[i].index;
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

void printC(Candidate *C, int totalCandidates) {
	fprintf(stdout, "printing C\n");
	int i;
	for (i = 0; i < totalCandidates; i++) {
		fprintf(stdout, "%d->%d ", C[i].index, C[i].distance);
	}
	fprintf(stdout, "\n");
}

unsigned int Dijkstra(Candidate *C, unsigned int *distances, int *parents, int *route, Vertex *vertices, int start, int end, int totalV, int totalE)
{
	if (start >= totalV || end >= totalV) {
		fprintf(stdout, "INF\n%d %d\n", start, end);
		return -1;
	}
	if (start == end) {
		fprintf(stdout, "0\n%d\n", start);
		return -1;
	}

	int totalCandidates = 0;
	
	initializeSingleSource(vertices, totalV, start, distances, parents, C, &totalCandidates);

	int length = 1;

	int i;

	//printC(C, totalCandidates);

	//exchangeQ(Q, start, length, totalV);
	//exchangeC(C, &totalCandidates, start);

	//printQ(Q, totalV);
	while (totalCandidates != 0) {
	//while (length != totalV) {
		int u = extractMinPoint(distances, C, totalCandidates);

		//fprintf(stdout, "chosen = %d\n", u);
	
		if (u == -1) {
			fprintf(stdout, "this is weird\n");
		}

		if (u == end) {
			totalCandidates = 0;
		}
		else {
			length += 1;
			exchangeC(C, &totalCandidates, u);

			//printC(C, totalCandidates);
			//printQ(Q, totalV);
			//printDistances(distances, totalV);

			for (i = 0; i < vertices[u].adjacentCount; i++) {
				int v = vertices[u].adjacentPoints[i];
				//fprintf(stdout, "total Candidates = %d\n", totalCandidates);
				int change = 0;
				relax(vertices, u, v, distances[u], distances, parents, &change);
				if (change != 0) {
					C[totalCandidates].distance = distances[v];
					C[totalCandidates++].index = v;
				}
			}

		}
	}

	//fprintf(stdout, "printing determiedn distances\n");
	//printDistances(distances, totalV);

	//fprintf(stdout, "total distance from %d to %d is %d\n", start, end, distances[end]);
	//fprintf(stdout, "%d\n", distances[end]);
	unsigned int sd = distances[end];

	printingShortestRoute(route, parents, start, end, totalV, sd); 

	return sd;
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
	//printVertices(vertices, totalV);

	readQueries(vertices, totalV, totalE, argv[2]);

	freeVertices(vertices, totalV);

	return EXIT_SUCCESS;
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

	//fprintf(stdout, "total number of queries: %d\n", totalQ);

	int i;

	Candidate *C = (Candidate *)malloc(sizeof(Candidate) * totalV);
	unsigned int *distances = (unsigned int *)malloc(sizeof(unsigned int) * totalV);
	int *parents = (int *)malloc(sizeof(int) * totalV);

	int *route = (int *)malloc(sizeof(int) * totalV);

	for (i = 0; i < totalQ; i++) {
		int start;
		int end;

		fscanf(fptr, "%d", &start);
		fscanf(fptr, "%d", &end);

		//fprintf(stdout, "***************************************start = %d and end = %d\n", start, end);

		Dijkstra(C, distances, parents, route, vertices, start, end, totalV, totalE);

		/*if (shortestDistance == -1) {
			fprintf(stdout, "!!!!!!!!!!!!!!!!!!Error, -1 returned on finding shortest path\n\n");
			//fclose(fptr);
			//return;
		}*/
	}

	free(C);
	free(distances);
	free(parents);
	free(route);
	fclose(fptr);

	return;
}

int distance(int x11, int y11, int x21, int y21)
{
	//fprintf(stdout, "calculating distances %d, %d, %d, %d\n", x1, y1, x2, y2);
	return (int) sqrt(pow((double)(x11 - x21), 2.0) + pow((double)(y11 - y21), 2.0));
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
	//fprintf(stdout, "doing this space efficiently!!\n");

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

/*void sortPointsList(Vertex *vertices, int totalV)
{
	int i;
	for (i = 0; i < totalV; i++) {
		quickSort(vertices[i].adjacentPoints, 0, vertices[i].adjacentCount - 1);	
	}
}*/
