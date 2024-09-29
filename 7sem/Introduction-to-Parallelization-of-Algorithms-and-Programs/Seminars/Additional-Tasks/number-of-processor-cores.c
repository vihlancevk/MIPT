#include <omp.h>
#include <stdio.h>

int main() {
	// cat /proc/cpuinfo | grep 'core id'
	int num_procs = omp_get_num_procs();
	printf("Number of processes: %d.\n", num_procs);

	return 0;
}
