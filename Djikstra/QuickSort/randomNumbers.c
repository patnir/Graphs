#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "Enter a number\n");
		return EXIT_FAILURE;
	}
	
	int numbers = atoi(argv[1]);
	if (numbers <= 0) {
		fprintf(stderr, "Enter a number greater than 0");
		return EXIT_FAILURE;
	}

	FILE *fptr = fopen("numbers.txt", "w+");

	if (fptr == NULL) {
		fprintf(stderr, "Error with opening file\n");	
	}

	srand(time(NULL));

	int *array = (int *) malloc(sizeof(int) * numbers);

	int i;

	for (i = 0; i < numbers; i++) {
		int r = rand();
		array[i] = r % 100;
		fprintf(stdout, "%d ", array[i]);
	}
	fprintf(stdout, "\n");
	

	fwrite(array, sizeof(int), numbers, fptr);

	fclose(fptr);

	return EXIT_SUCCESS;
}
