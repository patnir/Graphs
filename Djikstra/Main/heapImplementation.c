#include <stdio.h>
#include <stdlib.h>

void checkHeapProperty(Candidate *C, int totalCandidates) 
{
	fprintf(stdout, "checking heap property\n");
	int currIndex = totalCandidates;
	int parent =  currIndex / 2;
	while (parent > 0) {
		if (C[parent - 1].distance > C[currIndex - 1].distance) {
			fprintf(stdout, "\n\n\nProperty not maintained at %d and %d\n\n\n\n", parent - 1, currIndex - 1);
		}
		currIndex -= 1;
		parent = currIndex / 2;
	}
}

void downwardHeapify(Candidate *C, int totalCandidates, int index) 
{
	int temp = C[index].distance;
	int tempIndex = C[index].index;
	int notOrdered = 1;
	int parent = index;
	while ((notOrdered == 1) && (parent < totalCandidates / 2)) {
		int child = (parent + 1) * 2 - 1;
		if ((child < totalCandidates - 1) && (C[child].distance > C[child + 1].distance)) {
			child += 1;
		}
		if (temp <= C[child].distance) {
			notOrdered = 0;
		}
		else {
			C[parent].distance = C[child].distance;
			C[parent].index = C[child].index;
			parent = child;
		}
	}
	C[parent].distance = temp;
	C[parent].index = tempIndex;
}

int extractMinHeap(Candidate *C, int *totalCandidates) 
{
	if (totalCandidates == 0) {
		fprintf(stderr, "cannot extractMin when size is 0");
		return 0;
	}
	int toRet = C[0].index;
	*totalCandidates -= 1;
	C[0].index = C[*totalCandidates].index;
	C[0].distance = C[*totalCandidates].distance;
	int size = *totalCandidates;
	downwardHeapify(C, size, 0);
	return toRet;
}

void upwardHeapify(Candidate *C, int index) 
{
	int temp = C[index].distance;
	int tempIndex = C[index].index;
	int parent = (index + 1) / 2 - 1;
	int child = index;
	while ((parent >= 0) && (C[parent].distance > temp)) {
		C[child].distance = C[parent].distance;
		C[child].index = C[parent].index;
		child = parent;
		parent = (parent + 1) / 2 - 1;
	}
	C[child].distance = temp;
	C[child].index = tempIndex;
}

void insertHeap(Candidate *C, int *totalCandidates, int index, int distance)
{
	C[*totalCandidates].distance = distance;
	C[*totalCandidates].index = index;
	upwardHeapify(C, *totalCandidates);
	*totalCandidates += 1;

}

void heapify(Candidate *C, int totalCandidates) 
{
	int i;
	for (i = totalCandidates / 2; i >= 0; i--) {
		downwardHeapify(C, totalCandidates, i);
	}
}