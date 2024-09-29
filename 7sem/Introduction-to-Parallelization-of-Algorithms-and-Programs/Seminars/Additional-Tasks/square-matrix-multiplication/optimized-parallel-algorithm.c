#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "matrix/matrix.h"

struct matrix_block {
	int* ptr_block;
	int n;
	int m;
	int i;
	int j;
	int n1;
	int m1;
};

struct matrix_blocks {
	struct matrix_block* ptr_blocks;
	int n;
	int m;
};

struct matrix_blocks split_matrix_on_matrix_blocks(int n, int* matrix, int matrix_block_size) {
	int height_in_blocks = 0;
	if (n % matrix_block_size == 0)
		height_in_blocks = n / matrix_block_size;
	else
		height_in_blocks = n / matrix_block_size + 1;

	int width_in_blocks = height_in_blocks;
	int n_blocks = height_in_blocks * width_in_blocks;

    struct matrix_block* ptr_blocks = (struct matrix_block*) calloc(
		n_blocks, sizeof(struct matrix_block)
	);
    int k = 0;
    for (int i = 0; i < n; i += matrix_block_size) {
        for (int j = 0; j < n; j += matrix_block_size) {
            int n1 = 0;
            if (i + matrix_block_size < n)
                n1 = matrix_block_size;
            else
                n1 = n - i;

			int m1 = 0;
            if (j + matrix_block_size < n)
                m1 = matrix_block_size;
            else
                m1 = n - j;

			int* ptr_block = (int*) calloc(n1 * m1, sizeof(int));
			for (int i1 = 0; i1 < n1; i1++) {
				for (int j1 = 0; j1 < m1; j1++) {
					*(ptr_block + m1 * i1 + j1) = *(matrix + n * (i + i1) + (j + j1));
				}
			}

            struct matrix_block block = {ptr_block, n, n, i, j, n1, m1};
            *(ptr_blocks + k) = block;
            k++;
        }
    }

	struct matrix_blocks blocks = {ptr_blocks, height_in_blocks, width_in_blocks};
    return blocks;
}

void multiply(struct matrix_block* result_matrix_block,
			  struct matrix_block* left_matrix_block,
			  struct matrix_block* right_matrix_block) {
	for (int i = 0; i < left_matrix_block->n1; i++) {
		for (int j = 0; j < right_matrix_block->m1; j++) {
			for (int k = 0; k < left_matrix_block->m1; k++) {
				int* c_ij = result_matrix_block->ptr_block + result_matrix_block->m1 * i + j;
				int a_ik = *(left_matrix_block->ptr_block + left_matrix_block->m1 * i + k);
				int b_kj = *(right_matrix_block->ptr_block + right_matrix_block->m1 * k + j);
				*c_ij += a_ik * b_kj;
			}
		}
	}
}

void custom_free(int* left_matrix,
				 struct matrix_blocks left_matrix_blocks,
				 int* right_matrix,
				 struct matrix_blocks right_matrix_blocks,
				 int* result_matrix,
				 struct matrix_blocks result_matrix_blocks) {
	free(left_matrix);
	for (int i = 0; i < left_matrix_blocks.n; i++) {
		for (int j = 0; j < left_matrix_blocks.m; j++) {
			free((left_matrix_blocks.ptr_blocks + left_matrix_blocks.n * i + j)->ptr_block);
		}
	}
	free(left_matrix_blocks.ptr_blocks);

	free(right_matrix);
	for (int i = 0; i < right_matrix_blocks.n; i++) {
        for (int j = 0; j < right_matrix_blocks.m; j++) {
            free((right_matrix_blocks.ptr_blocks + right_matrix_blocks.n * i + j)->ptr_block);
        }
    }
	free(right_matrix_blocks.ptr_blocks);

	free(result_matrix);
	for (int i = 0; i < result_matrix_blocks.n; i++) {
        for (int j = 0; j < result_matrix_blocks.m; j++) {
            free((result_matrix_blocks.ptr_blocks + result_matrix_blocks.n * i + j)->ptr_block);
        }
    }
    free(result_matrix_blocks.ptr_blocks);
}

int main(int argc, char** argv) {
	int MATRIX_BLOCK_SIZE = 4;

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

	struct matrix_blocks left_matrix_blocks = split_matrix_on_matrix_blocks(
		n, left_matrix, MATRIX_BLOCK_SIZE
	);
	#ifdef DEBUG
        printf("left matrix blocks:\n");
        printf("n = %d, m = %d\n", left_matrix_blocks.n, left_matrix_blocks.m);
        for (int i = 0; i < left_matrix_blocks.n; i++) {
            for (int j = 0; j < left_matrix_blocks.m; j++) {
            	printf("matrix block:\n");
				printf("(%d,%d):\n", i, j);
				struct matrix_block* left_matrix_block = left_matrix_blocks.ptr_blocks +
					left_matrix_blocks.n * i +
					j;
				for (int i1 = 0; i1 < left_matrix_block->n1; i1++) {
					for (int j1 = 0; j1 < left_matrix_block->m1; j1++) {
						printf("%d ", *(left_matrix_block->ptr_block + left_matrix_block->m1 * i1 + j1));
					}
					printf("\n");
				}
			}
		}
	#endif

	int* right_matrix = read_square_matrix(n, argv[3]);
	if (right_matrix == 0) {
		return -1;
	}
	#ifdef DEBUG
        printf("right matrix:\n");
		print_square_matrix(n, right_matrix);
	#endif

	struct matrix_blocks right_matrix_blocks = split_matrix_on_matrix_blocks(
        n, right_matrix, MATRIX_BLOCK_SIZE
    );
    #ifdef DEBUG
        printf("right matrix blocks:\n");
        printf("n = %d, m = %d\n", right_matrix_blocks.n, right_matrix_blocks.m);
        for (int i = 0; i < right_matrix_blocks.n; i++) {
            for (int j = 0; j < right_matrix_blocks.m; j++) {
                printf("matrix block:\n");
                printf("(%d,%d):\n", i, j);
                struct matrix_block* right_matrix_block = right_matrix_blocks.ptr_blocks +
                    right_matrix_blocks.n * i +
                    j;
				for (int i1 = 0; i1 < right_matrix_block->n1; i1++) {
					for (int j1 = 0; j1 < right_matrix_block->m1; j1++) {
						printf("%d ", *(right_matrix_block->ptr_block + right_matrix_block->m1 * i1 + j1));
					}
					printf("\n");
				}
            }
        }
    #endif

	int* result_matrix = (int*) calloc(n*n, sizeof(int));
	#ifdef DEBUG
        printf("init: result matrix:\n");
        print_square_matrix(n, result_matrix);
    #endif

	struct matrix_blocks result_matrix_blocks = split_matrix_on_matrix_blocks(
        n, result_matrix, MATRIX_BLOCK_SIZE
    );
	#ifdef DEBUG
        printf("init: result matrix blocks:\n");
        printf("n = %d, m = %d\n", result_matrix_blocks.n, result_matrix_blocks.m);
        for (int i = 0; i < result_matrix_blocks.n; i++) {
            for (int j = 0; j < result_matrix_blocks.m; j++) {
                printf("matrix block:\n");
                printf("(%d,%d):\n", i, j);
                struct matrix_block* result_matrix_block = result_matrix_blocks.ptr_blocks +
                    result_matrix_blocks.n * i +
                    j;
				for (int i1 = 0; i1 < result_matrix_block->n1; i1++) {
					for (int j1 = 0; j1 < result_matrix_block->m1; j1++) {
						printf("%d ", *(result_matrix_block->ptr_block + result_matrix_block->m1 * i1 + j1));
					}
					printf("\n");
				}
            }
        }
    #endif

	#ifdef DEBUG
		assert(left_matrix_blocks.m == right_matrix_blocks.n);
	#endif

	time_t t0 = time(0);

	#pragma omp parallel for collapse(3)
	for (int i = 0; i < left_matrix_blocks.n; i++) {
		for (int j = 0; j < right_matrix_blocks.m; j++) {
			for (int k = 0; k < left_matrix_blocks.m; k++) {
				struct matrix_block* result_matrix_block = result_matrix_blocks.ptr_blocks + result_matrix_blocks.m * i + j;
				struct matrix_block* left_matrix_block = left_matrix_blocks.ptr_blocks + left_matrix_blocks.m * i + k;
				struct matrix_block* right_matrix_block = right_matrix_blocks.ptr_blocks + right_matrix_blocks.m * k + j;

				#ifdef DEBUG
					assert(left_matrix_block->m1 == right_matrix_block->n1);
				#endif

				multiply(result_matrix_block, left_matrix_block, right_matrix_block);
			}
		}
	}

	time_t t1 = time(0);
	double time_in_seconds = difftime(t1, t0);
	printf("time: %f\n", time_in_seconds);

	for (int i = 0; i < result_matrix_blocks.n; i++) {
		for (int j = 0; j < result_matrix_blocks.m; j++) {
			struct matrix_block* result_matrix_block = result_matrix_blocks.ptr_blocks + 
				result_matrix_blocks.n * i + 
				j;
			for (int i1 = 0; i1 < result_matrix_block->n1; i1++) {
				for (int j1 = 0; j1 < result_matrix_block->m1; j1++) {
					int* c_ij = result_matrix + 
						n * (result_matrix_block->i + i1) + 
						(result_matrix_block->j + j1);
					*c_ij = *(result_matrix_block->ptr_block + result_matrix_block->m1 * i1 + j1);
				}
			}
		}
	}

	#ifdef DEBUG
        printf("result matrix:\n");
		print_square_matrix(n, result_matrix);
	#endif

	int status = write_square_matrix(n, result_matrix, argv[4]);
	if (status != 0) {
		custom_free(
			left_matrix, left_matrix_blocks,
			right_matrix, right_matrix_blocks,
			result_matrix, result_matrix_blocks
		);
		return -1;
	}

	custom_free(
		left_matrix, left_matrix_blocks,
		right_matrix, right_matrix_blocks,
		result_matrix, result_matrix_blocks
	);
	return 0;
}
