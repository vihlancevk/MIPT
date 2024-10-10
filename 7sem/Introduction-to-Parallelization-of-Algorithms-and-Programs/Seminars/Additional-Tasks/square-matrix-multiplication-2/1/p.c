#include <omp.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../square-matrix-multiplication/matrix/matrix.h"

#define N 1024

// Function to horizontally sum all elements in a __m256i vector
int horizontal_sum_avx2(__m256i vec) {
    // Sum the lower and upper 128 bits separately
    __m128i low = _mm256_castsi256_si128(vec);       // Lower 128 bits
    __m128i high = _mm256_extracti128_si256(vec, 1); // Upper 128 bits

    // Add the two 128-bit lanes
    __m128i sum128 = _mm_add_epi32(low, high);

    // Perform horizontal additions within the 128-bit lane
    sum128 = _mm_hadd_epi32(sum128, sum128); // Sum pairs
    sum128 = _mm_hadd_epi32(sum128, sum128); // Sum the results

    // Extract the final sum
    return _mm_cvtsi128_si32(sum128);
}

void custom_free(int* left_matrix, int* right_matrix, int* transposed_right_matrix, int* result_matrix) {
    free(left_matrix);
    free(right_matrix);
    free(transposed_right_matrix);
    free(result_matrix);
}

int main(int argc, char** argv) {
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

	#pragma omp parallel for simd
	for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
			int* c_ij = result_matrix + N * i + j;
            for (int k = 0; k < N; k += 8) {
				__m256i vec_a = _mm256_loadu_si256((__m256i*) (left_matrix + N * i + k));
        		__m256i vec_b = _mm256_loadu_si256((__m256i*) (transposed_right_matrix + N * j + k));

				__m256i vec_result = _mm256_mullo_epi32(vec_a, vec_b);

				*c_ij += horizontal_sum_avx2(vec_result);
			}
        }
    }

	double end = omp_get_wtime();
    double time_in_seconds = end - start;
    printf("time: %f\n", time_in_seconds); 

	int status = write_square_matrix(N, result_matrix, argv[3]);
	if (status != 0) {
		custom_free(left_matrix, right_matrix, transposed_right_matrix, result_matrix);
		return -1;
	}

	custom_free(left_matrix, right_matrix, transposed_right_matrix, result_matrix);
	return 0;
}
