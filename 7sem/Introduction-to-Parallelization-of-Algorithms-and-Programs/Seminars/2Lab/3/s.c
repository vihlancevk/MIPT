#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ISIZE 5000
#define JSIZE 5000

int main(int argc, char **argv) {
	int i, j;
	double *a, *b;
	FILE *ff;

	a = (double *) calloc(ISIZE*JSIZE, sizeof(double));
	b = (double *) calloc(ISIZE*JSIZE, sizeof(double));
	for (i = 0; i < ISIZE; i++) {
		for (j = 0; j < JSIZE; j++) {
			*(a + JSIZE*i + j) = 10*i + j;
			*(b + JSIZE*i + j) = 0;
		}
	}

	double t_start = omp_get_wtime();

	for (i = 0; i < ISIZE; i++) {
		for (j = 0; j < JSIZE; j++) {
			double x = *(a + JSIZE*i + j);
			*(a + JSIZE*i + j) = sin(0.01*x);
		}
	}

	for (i = 0; i < ISIZE; i++) {
		for (j = 2; j < JSIZE; j++) {
			double x = *(a + JSIZE*i + (j - 2));
			*(b + JSIZE*i + j) = 2.5*x;
		}
	}

	double t_end = omp_get_wtime();
	double working_time = t_end - t_start;
	printf("%f\n", working_time);

	ff = fopen("s.txt", "w");
	for (i = 0; i < ISIZE; i++) {
		for (j = 0; j < JSIZE; j++) {
			fprintf(ff, "%f ", *(b + JSIZE*i + j));
		}
		fprintf(ff, "\n");
	}
	fclose(ff);

	return 0;
}
