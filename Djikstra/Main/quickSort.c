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