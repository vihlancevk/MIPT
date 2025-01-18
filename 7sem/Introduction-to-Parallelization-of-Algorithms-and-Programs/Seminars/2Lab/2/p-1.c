#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ISIZE 5000
#define JSIZE 5000

int main(int argc, char **argv) {
	int i, j;
	double *a, *a_copy;
	FILE *ff;

	a = (double *) calloc(ISIZE*JSIZE, sizeof(double));
	a_copy = (double *) calloc(ISIZE*JSIZE, sizeof(double));
	for (i = 0; i < ISIZE; i++) {
		for (j = 0; j < JSIZE; j++) {
			double x = 10*i + j;
			a[JSIZE*i + j] = x;
			a_copy[JSIZE*i + j] = x;
		}
	}

	double t_start = omp_get_wtime();

	#pragma omp parallel for
	for (i = 0; i < ISIZE - 1; i++) {
		for (j = 1; j < JSIZE; j++) {
			a[JSIZE*i + j] = sin(0.1 * a_copy[JSIZE*(i + 1) + (j - 1)]);
		}
	}

	double t_end = omp_get_wtime();
	double working_time = t_end - t_start;
	printf("%f\n", working_time);

	ff = fopen("p-1.txt", "w");
	for (i = 0; i < ISIZE; i++) {
		for (j = 0; j < JSIZE; j++) {
			fprintf(ff, "%f ", a[JSIZE*i + j]);
		}
		fprintf(ff, "\n");
	}
	fclose(ff);

	free(a);
	free(a_copy);
	return 0;
}
