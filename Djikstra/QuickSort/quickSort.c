#include <stdio.h>
#include <stdlib.h>

int choosePivot(int a, int b) {
	return (a + b) / 2;
}

void swap(int *A, int a, int b) {
	int temp = A[a];
	A[a] = A[b];
	A[b] = temp;
}

void quickSort(int *A, int left, int right) {
	if (left >= right) {
		return;
	}
	int pivot = choosePivot(left, right);
	swap(A, pivot, left);

	int j;
	int i = left + 1;
	for (j = left + 1; j <= right; j++) {
		if (A[j] < A[left]) {
			swap(A, i, j);
			i += 1;
		}
	}

	swap(A, left, i - 1);

	quickSort(A, left, i - 2);
	quickSort(A, i, right);
}


int *readFile(char *filename, int length) 
{
	FILE *fptr = (FILE*) fopen(filename, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error opening file\n");
	}
	int *numbers = (int *)malloc(sizeof(int) * length);
	fread(numbers, sizeof(int), length, fptr);
	return numbers;
}

void printArray(int *A, int length) 
{
	int i;
	for (i = 0; i < length; i++) {
		fprintf(stdout, "%d ", A[i]);
	}
	fprintf(stdout, "\n");
	return;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "invalid inputs\n");
		return EXIT_FAILURE;
	}

	int length = atoi(argv[2]);

	int *numbers = readFile(argv[1], length);

	printArray(numbers, length);

	quickSort(numbers, 0, length - 1);
	printArray(numbers, length);

	return EXIT_SUCCESS;
}
