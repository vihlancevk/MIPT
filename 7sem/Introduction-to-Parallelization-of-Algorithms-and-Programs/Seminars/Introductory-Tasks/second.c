#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	int N = atoi(argv[1]);

	int num_procs = omp_get_num_procs();
	double sums[num_procs];
	for (int i = 0; i < num_procs; ++i) {
		sums[i] = 0.0;
	}

	int num_threads = num_procs;
	#pragma omp parallel for num_threads(num_threads)
		for (int i = 1; i <= N; ++i) {
			int cur_thread = omp_get_thread_num();
			sums[cur_thread] += 1.0/i;
		}

	double sum = 0;
	for (int i = 0; i < num_procs; ++i) {
        sum += sums[i];
    }

	printf("Result: %f.\n", sum);

	return 0;
}
