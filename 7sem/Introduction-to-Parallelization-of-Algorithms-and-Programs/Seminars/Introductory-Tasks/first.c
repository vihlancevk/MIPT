#include <omp.h>
#include <stdio.h>

void report() {
	int position = 0;
	char buffer[512];

	int num_threads = omp_get_num_threads();
    position += snprintf(
		buffer + position,
		64,
		"Number of threads in parallel region: %d.\n",
		num_threads
	);

	int cur_thread = omp_get_thread_num();
    position += snprintf(
		buffer + position,
		64,
		"Number of current thread: %d.\n",
		cur_thread
	);

    position += snprintf(buffer + position, 64, "Hello World!\n");

	printf("%s", buffer);
}

int main() {
	int num_threads = omp_get_num_procs();
	#pragma omp parallel num_threads(num_threads)
	{
		report();
	}

	return 0;
}
