#ifndef MATRIX_H
#define MATRIX_H

int* read_matrix(int n, int m, char* file_name);

int* read_square_matrix(int n, char* file_name);

int write_matrix(int n, int m, int* matrix, char* file_name);

int write_square_matrix(int n, int* matrix, char* file_name);

void print_matrix(int n, int m, int* matrix);

void print_square_matrix(int n, int* matrix);

int* transpose_matrix(int n, int m, int* matrix);

int* transpose_square_matrix(int n, int* matrix);

#endif /* MATRIX_H */
