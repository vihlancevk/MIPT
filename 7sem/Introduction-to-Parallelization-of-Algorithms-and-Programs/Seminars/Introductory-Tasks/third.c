#include <omp.h>
#include <stdio.h>

int main() {
	int n = 0;

	#pragma omp parallel for ordered
		for (int i = 0; i < omp_get_num_threads(); i++) {
			#pragma omp ordered
        	{
				int thread_num = omp_get_thread_num();
				printf("Thread: %d, n: %d.\n", thread_num, n);
				n++;
			}
		}

	printf("%d\n", n);

	return 0;
}
