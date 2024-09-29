#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int* read_matrix(int n, int m, char* file_name) {
	int* matrix = (int*) calloc(n*m, sizeof(int));

	FILE *pf = fopen(file_name, "r");
    if (pf == 0) {
		free(matrix);
        return 0;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int* i_j = matrix + n * i + j;
			fscanf(pf, "%d", i_j);
		}
	}

	fclose(pf);
	return matrix;
}

int* read_square_matrix(int n, char* file_name) {
	return read_matrix(n, n, file_name);
}

int write_matrix(int n, int m, int* matrix, char* file_name) {
	FILE *pf = fopen(file_name, "w");
    if (pf == 0) {
        return -1;
    }

	for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int i_j = *(matrix + n * i + j);
            fprintf(pf, "%d", i_j);
			if (j != n - 1) {
				fprintf(pf, " ");
			}
        }
		fprintf(pf, "\n");
    }

	fclose(pf);
	return 0;
}

int write_square_matrix(int n, int* matrix, char* file_name) {
	return write_matrix(n, n, matrix, file_name);
}

void print_matrix(int n, int m, int* matrix) {
	for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
			int i_j = *(matrix + n * i + j);
            printf("%d ", i_j);
        }
		printf("\n");
    }
}

void print_square_matrix(int n, int* matrix) {
	print_matrix(n, n, matrix);
}

int* transpose_matrix(int n, int m, int* matrix) {
	int* transposed_matrix = (int*) calloc(n*n, sizeof(int));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			*(transposed_matrix + n * i + j) = *(matrix + n * j + i);
		}
	}

	return transposed_matrix;
}

int* transpose_square_matrix(int n, int* matrix) {
	return transpose_matrix(n, n, matrix);
}

