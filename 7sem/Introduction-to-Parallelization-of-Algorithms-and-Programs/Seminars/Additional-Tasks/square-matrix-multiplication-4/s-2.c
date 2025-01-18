#include <omp.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>

#define base_size 64

// Функция для сложения матриц с использованием AVX2 для целых чисел
void add_matrix_AVX(int *A, int *B, int *C, int size) {
    for (int i = 0; i < size * size; i += 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)&A[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&B[i]);
        __m256i c = _mm256_add_epi32(a, b);
        _mm256_storeu_si256((__m256i*)&C[i], c);
    }
}

// Функция для вычитания матриц с использованием AVX2 для целых чисел
void subtract_matrix_AVX(int *A, int *B, int *C, int size) {
	for (int i = 0; i < size * size; i += 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)&A[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&B[i]);
        __m256i c = _mm256_sub_epi32(a, b);
        _mm256_storeu_si256((__m256i*)&C[i], c);
    }
}

// Базовое умножение для малых матриц
void multiply(int *A, int *B, int *C) {
    for (int i = 0; i < base_size; i++) {
        for (int j = 0; j < base_size; j++) {
            C[i * base_size + j] = 0;
            for (int k = 0; k < base_size; k++) {
                C[i * base_size + j] += A[i * base_size + k] * B[k * base_size + j];
            }
        }
    }
}

// Рекурсивная реализация алгоритма Штрассена с AVX2 для целых чисел
void strassen_AVX(int *A, int *B, int *C, int size) {
    if (size <= base_size) {
        multiply(A, B, C);
        return;
    }

    int newSize = size / 2;
    int numElements = newSize * newSize;

    int *A11 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *A12 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *A21 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *A22 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *B11 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *B12 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *B21 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *B22 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *M1 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *M2 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *M3 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *M4 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *M5 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *M6 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *M7 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *temp1 = (int*)aligned_alloc(32, numElements * sizeof(int));
    int *temp2 = (int*)aligned_alloc(32, numElements * sizeof(int));

    // Заполнение подматриц A и B
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i * newSize + j] = A[i * size + j];
            A12[i * newSize + j] = A[i * size + j + newSize];
            A21[i * newSize + j] = A[(i + newSize) * size + j];
            A22[i * newSize + j] = A[(i + newSize) * size + j + newSize];
            B11[i * newSize + j] = B[i * size + j];
            B12[i * newSize + j] = B[i * size + j + newSize];
            B21[i * newSize + j] = B[(i + newSize) * size + j];
            B22[i * newSize + j] = B[(i + newSize) * size + j + newSize];
        }
    }

    // Вычисление промежуточных M1 - M7
	// M1 = (A11 + A22)(B11 + B22)
    add_matrix_AVX(A11, A22, temp1, newSize);
    add_matrix_AVX(B11, B22, temp2, newSize);
    strassen_AVX(temp1, temp2, M1, newSize);

	// M2 = (A21 + A22)B11
    add_matrix_AVX(A21, A22, temp1, newSize);
    strassen_AVX(temp1, B11, M2, newSize);

	// M3 = A11(B12 − B22)
    subtract_matrix_AVX(B12, B22, temp2, newSize);
    strassen_AVX(A11, temp2, M3, newSize);

	// M4 = A22(B21 − B11)
    subtract_matrix_AVX(B21, B11, temp2, newSize);
    strassen_AVX(A22, temp2, M4, newSize);

	// M5 = (A11 + A12)B22
    add_matrix_AVX(A11, A12, temp1, newSize);
    strassen_AVX(temp1, B22, M5, newSize);

	// M6 =(A21 − A11)(B11 + B12)
    subtract_matrix_AVX(A21, A11, temp1, newSize);
    add_matrix_AVX(B11, B12, temp2, newSize);
    strassen_AVX(temp1, temp2, M6, newSize);

	// M7 = (A12 − A22)(B21 + B22)
    subtract_matrix_AVX(A12, A22, temp1, newSize);
    add_matrix_AVX(B21, B22, temp2, newSize);
    strassen_AVX(temp1, temp2, M7, newSize);

    // Собираем итоговую матрицу C
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            int c11 = M1[i * newSize + j] + M4[i * newSize + j] - M5[i * newSize + j] + M7[i * newSize + j];
            int c12 = M3[i * newSize + j] + M5[i * newSize + j];
            int c21 = M2[i * newSize + j] + M4[i * newSize + j];
            int c22 = M1[i * newSize + j] - M2[i * newSize + j] + M3[i * newSize + j] + M6[i * newSize + j];
            C[i * size + j] = c11;
            C[i * size + j + newSize] = c12;
            C[(i + newSize) * size + j] = c21;
            C[(i + newSize) * size + j + newSize] = c22;
        }
    }

    // Освобождение памяти
    free(A11); free(A12); free(A21); free(A22);
    free(B11); free(B12); free(B21); free(B22);
    free(M1); free(M2); free(M3); free(M4);
    free(M5); free(M6); free(M7);
    free(temp1); free(temp2);
}

// Функция для вывода матрицы
void print_matrix(int *matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i * size + j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int n = 4096;  // Размер матрицы (должен быть степенью 2)

	int *A = (int*)calloc(n*n, sizeof(int));
    int *B = (int*)calloc(n*n, sizeof(int));
    int *C = (int*)calloc(n*n, sizeof(int));
	// int *C_test = (int*)calloc(n*n, sizeof(int));

    // Инициализация матриц
    srand(0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[n*i + j] = rand() % 10;
            B[n*i + j] = rand() % 10;
        }
    }

	printf("base size: %d\n", base_size);

    double start = omp_get_wtime();

    // Вызов алгоритма Штрассена
    strassen_AVX(A, B, C, n);

    double end = omp_get_wtime();
    double time_in_seconds = end - start;
    printf("time: %f\n", time_in_seconds);

    FILE *pf = fopen("s-2.txt", "w");
    if (pf != 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(pf, "%d ", C[n*i + j]);
            }
            fprintf(pf, "\n");
        }

        fclose(pf);
    }

//	for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//            for (int k = 0; k < n; k++) {
//				C_test[n*i + j] += A[n*i + k] * B[n*k + j];
//            }
//        }
//    }	
//
//	FILE *pf_test = fopen("s-2-test.txt", "w");
//    if (pf_test != 0) {
//        for (int i = 0; i < n; i++) {
//            for (int j = 0; j < n; j++) {
//                fprintf(pf_test, "%d ", C_test[n*i + j]);
//            }
//            fprintf(pf_test, "\n");
//        }
//
//        fclose(pf_test);
//    }

    // Освобождение памяти
    free(A);
    free(B);
    free(C);
	// free(C_test);

    return 0;
}

