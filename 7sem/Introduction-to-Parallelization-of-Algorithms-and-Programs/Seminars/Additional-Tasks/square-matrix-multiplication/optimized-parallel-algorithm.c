#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix/matrix.h"

#define SM 16

void custom_free(int* left_matrix, int* right_matrix, int* result_matrix) {
	free(left_matrix);
	free(right_matrix);
	free(result_matrix);
}

int main(int argc, char** argv) {
	int n = atoi(argv[1]);
	if (n < SM || n % SM != 0) {
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

	int* result_matrix = (int*) calloc(n*n, sizeof(int));
	if (result_matrix == 0) {
		return -1;
	}

	double start = omp_get_wtime();

	#pragma omp parallel for
	for (int i = 0; i < n; i += SM) {
		for (int j = 0; j < n; j += SM) {
			for (int k = 0; k < n; k += SM) {
				int* result = result_matrix + n*i + j;
				int* left = left_matrix + n*i + k;
				for (int i2 = 0; i2 < SM; i2++, result += n, left += n) {
					int* right = right_matrix + n*k + j;
					for (int k2 = 0; k2 < SM; k2++, right += n) {
						for (int j2 = 0; j2 < SM; j2++) {
                        	int a = *(left + k2);
                            int b = *(right + j2);
                            *(result + j2) += a * b;
						}
					}
				}
			}
		}
	}

	double end = omp_get_wtime();
    double time_in_seconds = end - start;
    printf("time: %f\n", time_in_seconds);

	#ifdef DEBUG
        printf("result matrix:\n");
		print_square_matrix(n, result_matrix);
	#endif

	int status = write_square_matrix(n, result_matrix, argv[4]);
	if (status != 0) {
		custom_free(left_matrix, right_matrix, result_matrix);
		return -1;
	}

	custom_free(left_matrix, right_matrix, result_matrix);
	return 0;
}
