#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ISIZE 5000
#define JSIZE 5000

int main(int argc, char **argv) {
	int i, j;
	double *a;
	FILE *ff;

	a = (double *) calloc(ISIZE*JSIZE, sizeof(double));
	for (i = 0; i < ISIZE; i++) {
		for (j = 0; j < JSIZE; j++) {
			*(a + JSIZE*i + j) = 10*i + j;
		}
	}

	double t_start = omp_get_wtime();

	for (i = 0; i < ISIZE - 1; i++) {
		#pragma omp parallel for
		for (j = 1; j < JSIZE; j++) {
			double x = *(a + JSIZE*(i + 1) + (j - 1));
			*(a + JSIZE*i + j) = sin(0.1 * x);
		}
	}

	double t_end = omp_get_wtime();
	double working_time = t_end - t_start;
	printf("%f\n", working_time);

	ff = fopen("p.txt", "w");
	for (i = 0; i < ISIZE; i++) {
		for (j = 0; j < JSIZE; j++) {
			fprintf(ff, "%f ", *(a + JSIZE*i + j));
		}
		fprintf(ff, "\n");
	}
	fclose(ff);

	return 0;
}
