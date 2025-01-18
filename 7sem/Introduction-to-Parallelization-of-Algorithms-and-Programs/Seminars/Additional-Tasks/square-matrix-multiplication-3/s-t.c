#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "../square-matrix-multiplication/matrix/matrix.h"

extern void multiply(int A[], int B[], int C[], int N);

void multiply(int A[], int B[], int C[], int N) {
	for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[N*i + j] += A[N*i + k] * B[N*j + k];
            }
        }
    }
}

void custom_free(int* left_matrix, int* right_matrix, int* result_matrix) {
	free(left_matrix);
	free(right_matrix);
	free(result_matrix);
}

int main(int argc, char** argv) {
	int N = 1024;

	int* left_matrix = read_square_matrix(N, argv[1]);
    if (left_matrix == 0) {
        return -1;
    }
	
	int* right_matrix = read_square_matrix(N, argv[2]);
	if (right_matrix == 0) {
		return -1;
	}

	int* transposed_right_matrix = transpose_square_matrix(N, right_matrix);
	
	int* result_matrix = (int*) calloc(N*N, sizeof(int));
	if (result_matrix == 0) {
		return -1;
	}
	
	double start = omp_get_wtime();

	multiply(left_matrix, transposed_right_matrix, result_matrix, N);

	double end = omp_get_wtime();
    double time_in_seconds = end - start;
    printf("time: %f\n", time_in_seconds); 

	int status = write_square_matrix(N, result_matrix, argv[3]);
	if (status != 0) {
		custom_free(left_matrix, right_matrix, result_matrix);
		free(transposed_right_matrix);
		return -1;
	}

	custom_free(left_matrix, right_matrix, result_matrix);
	free(transposed_right_matrix);
	return 0;
}
