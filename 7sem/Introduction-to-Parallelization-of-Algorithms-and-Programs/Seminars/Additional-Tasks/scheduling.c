#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_config(int num_threads, char* scheduling_type, int chunk_size, int num_iterations) {
	printf("Number of threads: %d.\n", num_threads);
	printf("Scheduling type: %s, chunk size: %d.\n", scheduling_type, chunk_size);
	printf("Number of iterations: %d.\n", num_iterations);
}

void print_info(int num_thread, int num_iteration) {
	printf("Number of thread: %d, number of iteration: %d.\n", num_thread, num_iteration);
}

// https://610yilingliu.github.io/2020/07/15/ScheduleinOpenMP/
int main(int argc, char** argv) {
	int num_threads = 4;
    omp_set_num_threads(num_threads);

	int chunk_size = atoi(argv[2]);
    int num_iterations = 65;

	if (strcmp(argv[1], "s") == 0) {
		print_config(num_threads, "static", chunk_size, num_iterations);
		#pragma omp parallel for schedule(static, chunk_size)
			for (int i = 0; i < num_iterations; i++) {
        		print_info(omp_get_thread_num(), i);
    		}
	}

	if (strcmp(argv[1], "d") == 0) {
        print_config(num_threads, "dynamic", chunk_size, num_iterations);
        #pragma omp parallel for schedule(dynamic, chunk_size)
            for (int i = 0; i < num_iterations; i++) {
                print_info(omp_get_thread_num(), i);
            }
    }

	if (strcmp(argv[1], "g") == 0) {
        print_config(num_threads, "guided", chunk_size, num_iterations);
        #pragma omp parallel for schedule(guided, chunk_size)
            for (int i = 0; i < num_iterations; i++) {
                print_info(omp_get_thread_num(), i);
            }
    }

	return 0;
}
