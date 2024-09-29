#include <omp.h>
#include <stdio.h>

int a = 0;
#pragma omp threadprivate(a)

// https://www.openmp.org/wp-content/uploads/OpenMP-4.0-C.pdf
// copyin(list)
// Copies the value of the master thread’s threadprivate
// variable to the threadprivate variable of each other
// member of the team executing the parallel region.
void copyin() {
	printf("1. copyin:\n");

    #pragma omp parallel
    {
        #pragma omp copyin(a)

        int num_thread = omp_get_thread_num();
		a += num_thread;
        printf("Thread %d: a = %d.\n", num_thread, a);
    }
}

int b = 0;
#pragma omp threadprivate(b)

// https://www.openmp.org/wp-content/uploads/OpenMP-4.0-C.pdf
// copyprivate(list)
// Broadcasts a value from the data environment of one
// implicit task to the data environments of the other
// implicit tasks belonging to the parallel region.
void copyprivate() {
	printf("2. copyprivate:\n");

	#pragma omp parallel
    {
		#pragma omp single copyprivate(b)
        {
			int num_thread = omp_get_thread_num();
			printf("!!!Single thread %d!!!\n", num_thread);
            b += 993;
        }

        int num_thread = omp_get_thread_num();
        printf("Thread %d: b = %d.\n", num_thread, b);
	}
	
}

int main() {
	copyin();
	copyprivate();

	return 0;
}
