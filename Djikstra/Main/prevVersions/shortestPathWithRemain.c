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
} Remain;

void sortPointsList(Vertex *vertices, int totalV);
void freeVertices(Vertex *vertices, int totalV);

Vertex *buildAdjacencyMatrix(char *filename, int *totalV, int *totalE);
Vertex *buildSpaceEfficientAdjacencyMatrix(char *filename, int *totalV, int *totalE);
Vertex *buildAdjacencyList(char *filename, int *totalV, int *totalE);

void printVertices(Vertex *vertices, int totalV);
void readQueries(Vertex *vertices, int totalV, int totalE, char *filename);
int distanceBetweenTwoPoints(int x1, int y1, int x2, int y2);

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

void relax(Vertex *vertices, int u, int v, int unsigned distanceU, unsigned int *distances, int *parents) 
{
	int distanceToAdd = distanceBetweenTwoPoints(vertices[u].x, vertices[u].y, vertices[v].x, vertices[v].y);
	if (distances[v] > distanceU + distanceToAdd) {
		distances[v] = distanceU + distanceToAdd;
		parents[v] = u;
	}
}

void initializeSingleSource(Vertex *vertices, int totalV, int start, unsigned int *distances, int *parents)
{
	int i;
	for (i = 0; i < totalV; i++) {
		distances[i] = INT_MAX;
		parents[i] = -1;
	}

	distances[start] = 0;

	for (i = 0; i < vertices[start].adjacentCount; i++) {
		int v = vertices[start].adjacentPoints[i];
		relax(vertices, start, v, 0, distances, parents);
	}
}

void exchangeQ(Remain *R, int index, int length, int totalV) 
{
	if (R[index].index != index) {
		int i = 0;
		while (index != R[i].index){
			fprintf(stdout, "asdf");
			i++;
		}
		index = i;
	}
	fprintf(stdout, "exchanging %d and %d\n", index, totalV - length);
	/*int temp = R[totalV - length].index;
	R[totalV - length].index = R[index].index;
	R[index].index = temp;
	unsigned int distance = R[totalV - length].distance;
	R[totalV - length].distance = R[index].distance;
	R[index].distance = distance;*/

	Remain temp = R[totalV-length];
	R[totalV-length] = R[index];
	R[index] = temp;
}

int extractMinPoint(unsigned int *distances, Remain *R, int length, int totalV) 
{
	int i;
	int minPoint = -1;
	unsigned int minDistance = INT_MAX;
	for (i = 0; i < totalV - length; i++) {
		if (minDistance > distances[R[i].index]) {
			minDistance = distances[R[i].index];
			minPoint = R[i].index;
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

void printQ(Remain *R, int totalV) {
	fprintf(stdout, "printing Remain\n");
	int i;
	for (i = 0; i < totalV; i++) {
		fprintf(stdout, "%d->%d ", R[i].index, R[i].distance);
	}
	fprintf(stdout, "\n");
}

unsigned int Dijkstra(Vertex *vertices, int start, int end, int totalV, int totalE)
{
	unsigned int *distances = (unsigned int *)malloc(sizeof(unsigned int) * totalV);
	int *parents = (int *)malloc(sizeof(int) * totalV);
	Remain *R = (Remain *)malloc(sizeof(Remain) * totalV);

	initializeSingleSource(vertices, totalV, start, distances, parents);

	int length = 1;

	int i;

	for (i = 0; i < totalV; i++) {
		R[i].index = i;
		R[i].distance = INT_MAX;
	}

	R[start].distance = 0;
	exchangeQ(R, start, length, totalV);

	printQ(R, totalV);
	while (length != totalV) {
		int u = extractMinPoint(distances, R, length, totalV);

		fprintf(stdout, "chosen = %d\n", u);

		length += 1;
		exchangeQ(R, u, length, totalV);

		printQ(R, totalV);
		//printDistances(distances, totalV);


		for (i = 0; i < vertices[u].adjacentCount; i++) {
			int v = vertices[u].adjacentPoints[i];
			relax(vertices, u, v, distances[u], distances, parents);
		}
	}

	fprintf(stdout, "printing determiedn distances\n");
	printDistances(distances, totalV);

	fprintf(stdout, "total distance from %d to %d is %d\n", start, end, distances[end]);
	unsigned int sd = distances[end];

	printingShortestRoute(parents, start, end, totalV); 

	free(R);
	free(distances);
	free(parents);

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
	printVertices(vertices, totalV);

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

int distanceBetweenTwoPoints(int x1, int y1, int x2, int y2)
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

/*void sortPointsList(Vertex *vertices, int totalV)
{
	int i;
	for (i = 0; i < totalV; i++) {
		quickSort(vertices[i].adjacentPoints, 0, vertices[i].adjacentCount - 1);	
	}
}*/