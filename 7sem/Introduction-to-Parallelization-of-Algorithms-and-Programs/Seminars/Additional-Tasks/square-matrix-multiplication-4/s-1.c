#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define base_size 32

void multiply(int **A, int **B, int **C) {
	for (int i = 0; i < base_size; i++) {
        for (int j = 0; j < base_size; j++) {
            for (int k = 0; k < base_size; k++) {
				C[i][j] += A[i][k] * B[k][j];
            }
        }
    }		
}

void add(int **A, int **B, int **C, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subtract(int **A, int **B, int **C, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            C[i][j] = A[i][j] - B[i][j];
}

void strassen(int **A, int **B, int **C, int size) {
    if (size == base_size) {
        multiply(A, B, C);
        return;
    }

    int newSize = size / 2;
    
    // Создание подматриц
    int **A11 = calloc(newSize, sizeof(int *));
    int **A12 = calloc(newSize, sizeof(int *));
    int **A21 = calloc(newSize, sizeof(int *));
    int **A22 = calloc(newSize, sizeof(int *));
    int **B11 = calloc(newSize, sizeof(int *));
    int **B12 = calloc(newSize, sizeof(int *));
    int **B21 = calloc(newSize, sizeof(int *));
    int **B22 = calloc(newSize, sizeof(int *));
    int **C11 = calloc(newSize, sizeof(int *));
    int **C12 = calloc(newSize, sizeof(int *));
    int **C21 = calloc(newSize, sizeof(int *));
    int **C22 = calloc(newSize, sizeof(int *));
    int **M1 = calloc(newSize, sizeof(int *));
    int **M2 = calloc(newSize, sizeof(int *));
    int **M3 = calloc(newSize, sizeof(int *));
    int **M4 = calloc(newSize, sizeof(int *));
    int **M5 = calloc(newSize, sizeof(int *));
    int **M6 = calloc(newSize, sizeof(int *));
    int **M7 = calloc(newSize, sizeof(int *));
    int **temp1 = calloc(newSize, sizeof(int *));
    int **temp2 = calloc(newSize, sizeof(int *));
    
    for (int i = 0; i < newSize; i++) {
        A11[i] = calloc(newSize, sizeof(int));
        A12[i] = calloc(newSize, sizeof(int));
        A21[i] = calloc(newSize, sizeof(int));
        A22[i] = calloc(newSize, sizeof(int));
        B11[i] = calloc(newSize, sizeof(int));
        B12[i] = calloc(newSize, sizeof(int));
        B21[i] = calloc(newSize, sizeof(int));
        B22[i] = calloc(newSize, sizeof(int));
        C11[i] = calloc(newSize, sizeof(int));
        C12[i] = calloc(newSize, sizeof(int));
        C21[i] = calloc(newSize, sizeof(int));
        C22[i] = calloc(newSize, sizeof(int));
        M1[i] = calloc(newSize, sizeof(int));
        M2[i] = calloc(newSize, sizeof(int));
        M3[i] = calloc(newSize, sizeof(int));
        M4[i] = calloc(newSize, sizeof(int));
        M5[i] = calloc(newSize, sizeof(int));
        M6[i] = calloc(newSize, sizeof(int));
        M7[i] = calloc(newSize, sizeof(int));
        temp1[i] = calloc(newSize, sizeof(int));
        temp2[i] = calloc(newSize, sizeof(int));
    }

    // Заполнение подматриц
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    // Вычисление M1 = (A11 + A22) * (B11 + B22)
    add(A11, A22, temp1, newSize);
    add(B11, B22, temp2, newSize);
    strassen(temp1, temp2, M1, newSize);

    // Вычисление M2 = (A21 + A22) * B11
    add(A21, A22, temp1, newSize);
    strassen(temp1, B11, M2, newSize);

    // Вычисление M3 = A11 * (B12 - B22)
    subtract(B12, B22, temp2, newSize);
    strassen(A11, temp2, M3, newSize);

    // Вычисление M4 = A22 * (B21 - B11)
    subtract(B21, B11, temp2, newSize);
    strassen(A22, temp2, M4, newSize);

    // Вычисление M5 = (A11 + A12) * B22
    add(A11, A12, temp1, newSize);
    strassen(temp1, B22, M5, newSize);

    // Вычисление M6 = (A21 - A11) * (B11 + B12)
    subtract(A21, A11, temp1, newSize);
    add(B11, B12, temp2, newSize);
    strassen(temp1, temp2, M6, newSize);

    // Вычисление M7 = (A12 - A22) * (B21 + B22)
    subtract(A12, A22, temp1, newSize);
    add(B21, B22, temp2, newSize);
    strassen(temp1, temp2, M7, newSize);

    // Вычисление подматриц C
    add(M1, M4, temp1, newSize);
    subtract(temp1, M5, temp2, newSize);
    add(temp2, M7, C11, newSize);

    add(M3, M5, C12, newSize);

    add(M2, M4, C21, newSize);

    add(M1, M3, temp1, newSize);
    subtract(temp1, M2, temp2, newSize);
    add(temp2, M6, C22, newSize);

    // Сборка результата в C
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }
    }

    // Освобождение памяти
    for (int i = 0; i < newSize; i++) {
        free(A11[i]); free(A12[i]); free(A21[i]); free(A22[i]);
        free(B11[i]); free(B12[i]); free(B21[i]); free(B22[i]);
        free(C11[i]); free(C12[i]); free(C21[i]); free(C22[i]);
        free(M1[i]); free(M2[i]); free(M3[i]); free(M4[i]);
        free(M5[i]); free(M6[i]); free(M7[i]); free(temp1[i]); free(temp2[i]);
    }
    free(A11); free(A12); free(A21); free(A22);
    free(B11); free(B12); free(B21); free(B22);
    free(C11); free(C12); free(C21); free(C22);
    free(M1); free(M2); free(M3); free(M4);
    free(M5); free(M6); free(M7); free(temp1); free(temp2);
}

int main(int argc, char **argv) {
	int n = 4096;  // Размер матрицы (должен быть степенью 2)
    
    // Пример матриц
    int **A = calloc(n, sizeof(int *));
    int **B = calloc(n, sizeof(int *));
    int **C = calloc(n, sizeof(int *));
    
    for (int i = 0; i < n; i++) {
        A[i] = calloc(n, sizeof(int));
        B[i] = calloc(n, sizeof(int));
        C[i] = calloc(n, sizeof(int));
    }

    // Инициализация матриц
	srand(0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

	double start = omp_get_wtime();

	// Вызов алгоритма Штрассена
    strassen(A, B, C, n);

	double end = omp_get_wtime();
    double time_in_seconds = end - start;
    printf("time: %f\n", time_in_seconds);

	FILE *pf = fopen("s-1.txt", "w");
    if (pf != 0) {
    	for (int i = 0; i < n; i++) {
        	for (int j = 0; j < n; j++) {
				fprintf(pf, "%d ", C[i][j]);
        	}
			fprintf(pf, "\n");
    	}

    	fclose(pf);
	}

    // Освобождение памяти
    for (int i = 0; i < n; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
