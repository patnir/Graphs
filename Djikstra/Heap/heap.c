#include<stdio.h>
#include<stdlib.h>
#include<time.h>


typedef struct {
	int *array; 
	int size;
	int limit;
} Heap;


void upwardHeapify(Heap *h, int index);
void heapify(Heap *h, int *array, int length); 

Heap *initializeHeap() 
{
	Heap *h = (Heap *)malloc(sizeof(Heap));
	h->size = 0;
	h->limit = 30;
	h->array = (int *)malloc(sizeof(int) * h->limit);
	return h;
}

void addSpace(Heap *h) 
{
	h->limit += 30;
	int i;
	int *newArray = (int *)malloc(sizeof(int) * h->limit);
	for (i = 0; i < h->size; i++) {
		newArray[i] = h->array[i];
	}
	free(h->array);
	h->array = newArray;
}

void checkHeapProperty(Heap *h) 
{
	fprintf(stdout, "checking heap property\n");
	int currIndex = h->size;
	int parent =  currIndex / 2;
	while (parent > 0) {
		if (h->array[parent - 1] > h->array[currIndex - 1]) {
			fprintf(stdout, "\n\n\nProperty not maintained at %d and %d\n\n\n\n", parent - 1, currIndex - 1);
		}
		currIndex -= 1;
		parent = currIndex / 2;
	}
}


void insert(Heap *h, int n)
{
	h->array[h->size] = n;
	upwardHeapify(h, h->size);
	h->size += 1;
	if (h->size == h->limit) {
		addSpace(h);
	}
	//checkHeapProperty(h);
}

void upwardHeapify(Heap *h, int index) 
{
	int temp = h->array[index];
	int parent = (index + 1) / 2 - 1;
	int child = index;
	while ((parent >= 0) && (h->array[parent] > temp)) {
		h->array[child] = h->array[parent];
		child = parent;
		parent = (parent + 1) / 2 - 1;
	}
	h->array[child] = temp;
}

void downwardHeapify(Heap *h, int index) 
{
	int temp = h->array[index];
	int notOrdered = 1;
	int parent = index;
	while ((notOrdered == 1) && (parent < h->size / 2)) {
		int child = (parent + 1) * 2 - 1;
		if ((child < h->size - 1) && (h->array[child] > h->array[child + 1])) {
			child += 1;
		}
		if (temp <= h->array[child]) {
			notOrdered = 0;
		}
		else {
			h->array[parent] = h->array[child];
			parent = child;
		}
	}
	h->array[parent] = temp;
}

int extractMin(Heap *h) 
{
	if (h->size == 0) {
		fprintf(stderr, "cannot extractMin when size is 0");
		return 0;
	}
	int toRet = h->array[0];
	h->size -= 1;
	h->array[0] = h->array[h->size];
	downwardHeapify(h, 0);
	return toRet;
}

void delete(Heap *h, int target) 
{
	if (h->size == 0) {
		fprintf(stderr, "Cannot delete when size is 0\n");
		return;
	}

	int i = 0;
	//int found = 0;
	int index = -1;
	while ((index < 0) && (i < h->size)) {
		if (target == h->array[i]) {
			index = i;
		}
		i += 1;
	}

	if (index < 0) {
		fprintf(stdout, "target not found\n");
		return;
	}

	h->size -= 1;
	h->array[index] = h->array[h->size];
	downwardHeapify(h, index);
}


void printHeap(Heap *h) 
{
	int i;
	fprintf(stdout, "size = %d limit = %d \narray = ", h->size, h->limit);
	for (i = 0 ; i < h->size; i++) {
		fprintf(stdout, "%d ", h->array[i]);
	}
	fprintf(stdout, "\n");
}


void createHeap(int size)
{
	if (size == 0) {
		return;
	}
	int i;
	srand(time(NULL));
	Heap *h = initializeHeap();
	
	int *insertions = (int *)malloc(sizeof(int) * size);
	
	for (i = 0; i < size; i++) {
		int r = rand();
		r = r % 100;
		fprintf(stdout, "adding %d to heap\n", r);
		insert(h, r);
		printHeap(h);
		checkHeapProperty(h);
		insertions[i] = r;
	}
	/*for (i = 0; i < size; i++) {
		int val = extractMin(h);
		fprintf(stdout, "minvalue = %d\n", val);
		printHeap(h);
		checkHeapProperty(h);
	}*/

	for (i = 0; i < size; i++) {
		delete(h, insertions[i]);	
		fprintf(stdout, "deleting = %d\n", insertions[i]);
		printHeap(h);
		checkHeapProperty(h);
	}

	Heap *newHeap = initializeHeap();

	heapify(newHeap, insertions, size);

	printHeap(newHeap);
	checkHeapProperty(newHeap);

	free(insertions);
	free(h);

	

	return;
}

void heapify(Heap *h, int *array, int length) 
{
	if (h->size != 0) {
		fprintf(stderr, "this does not work for already filled heaps\n");
		return;
	}

	int i;

	h->size = length;

	for (i = 0; i < length; i++) {
		h->array[i] = array[i];
	}

	for (i = length / 2; i >= 0; i--) {
		downwardHeapify(h, i);
	}
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Enter a number for the size of heap\n");
		return EXIT_FAILURE;
	}
	createHeap(atoi(argv[1]));
	return EXIT_SUCCESS;
}	
