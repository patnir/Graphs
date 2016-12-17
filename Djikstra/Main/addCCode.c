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
