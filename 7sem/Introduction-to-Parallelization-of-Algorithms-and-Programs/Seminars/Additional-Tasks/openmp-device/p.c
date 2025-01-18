#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#pragma omp declare target
extern double calculate_sum(double A[], size_t N);
#pragma omp end declare target

double calculate_sum(double A[], size_t N) {
    double sum = 0;

	#pragma omp target map(to: A[:N]) map(tofrom: sum)
	{
		#pragma omp parallel for reduction(+:sum)
		for (size_t i = 0; i < N; i++) {
			sum += A[i];
		}
	}

    return sum;
}

int main() {
	size_t N = 2 << 28;
	double *A = (double *) calloc(N, sizeof(double));
	for (size_t i = 0; i < N; i++) {
		*(A + i) = 1;
	}

	double t_start = omp_get_wtime();

	double sum = calculate_sum(A, N);
	double average = sum / N;

	double t_end = omp_get_wtime();
	double working_time = t_end - t_start;
	printf("time: %f\n", working_time);

	printf("average: %f\n", average);

	free(A);
	return 0;
}
