#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix/matrix.h"

void custom_free(int* left_matrix, int* right_matrix, int* transposed_right_matrix, int* result_matrix) {
	free(left_matrix);
	free(right_matrix);
	free(transposed_right_matrix);
	free(result_matrix);
}

int main(int argc, char** argv) {
	int n = atoi(argv[1]);
	if (n < 1) {
		return -1;
	}

	int* left_matrix = read_square_matrix(n, argv[2]);
    if (left_matrix == 0) {
        return -1;
    }
	#ifdef DEBUG
		printf("left matrix:\n");
        print_square_matrix(n, left_matrix);
	#endif

	int* right_matrix = read_square_matrix(n, argv[3]);
	if (right_matrix == 0) {
		return -1;
	}
	#ifdef DEBUG
        printf("right matrix:\n");
		print_square_matrix(n, right_matrix);
	#endif

	int* transposed_right_matrix = transpose_square_matrix(n, right_matrix);
	#ifdef DEBUG
        printf("transposed right matrix:\n");
        print_square_matrix(n, transposed_right_matrix);
    #endif

	int* result_matrix = (int*) calloc(n*n, sizeof(int)); 
	
	time_t t0 = time(0);

	#pragma omp parallel for collapse(3)
	for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
				int* c_ij = result_matrix + n * i + j;
				int a_ik = *(left_matrix + n * i + k);
				int b_jk = *(transposed_right_matrix + n * j + k);
				*c_ij += a_ik * b_jk;
			}
        }
    }

	time_t t1 = time(0);
    double time_in_seconds = difftime(t1, t0);
    printf("time: %f\n", time_in_seconds); 

	#ifdef DEBUG
        printf("result matrix:\n");
		print_square_matrix(n, result_matrix);
	#endif

	int status = write_square_matrix(n, result_matrix, argv[4]);
	if (status != 0) {
		custom_free(left_matrix, right_matrix, transposed_right_matrix, result_matrix);
		return -1;
	}

	custom_free(left_matrix, right_matrix, transposed_right_matrix, result_matrix);
	return 0;
}
