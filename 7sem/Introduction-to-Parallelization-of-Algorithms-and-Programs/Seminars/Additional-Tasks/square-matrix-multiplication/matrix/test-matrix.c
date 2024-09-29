#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

void test_transpose_matrix(int n, int m, int* matrix) {
	printf("Test transpose.\n");

	printf("Before:\n");
	print_matrix(n, m, matrix);

	int* transposed_matrix = transpose_matrix(n, m, matrix);
	printf("After:\n");
	print_matrix(n, m, transposed_matrix);
	free(transposed_matrix);	
}

void main(int argc, char** argv) {
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	int* matrix = read_matrix(n, m, argv[3]);

	test_transpose_matrix(n, m, matrix);

	free(matrix);
}
