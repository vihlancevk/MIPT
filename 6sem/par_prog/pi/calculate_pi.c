#include "mpi.h"
#include <stdio.h>
#include <math.h>

#define PI 3.141592653589793238462643

int enter_n_of_intervals() {
	return 500;
}

int main(int argc, char *argv[]) {
	int n_procs, cur_proc, n_of_intervals, i;
	double res_pi, cur_res_pi, h, sum, x, error;

	if (n_of_intervals <= 0) {
        printf("Entered incorrect number of intervals: %d.\n", n_of_intervals);
        return 0;
    }

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &cur_proc);

	h = 1.0 / (double) n_of_intervals;

	sum = 0.0;

	for (i = cur_proc + 1; i <= n_of_intervals; i += n_procs) {
		x = h * ((double) i - 0.5);
		sum += (4.0 / (1.0 + x * x));
	}

	cur_res_pi = h * sum;

	MPI_Reduce(&cur_res_pi, &res_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (cur_proc == 0) {
		error = (fabs(res_pi - PI) / PI) * 100;
		printf("pi is approximately %.16f. Error is %.6f%%.\n", res_pi, error);
	}

	MPI_Finalize();

	return 0;
}

