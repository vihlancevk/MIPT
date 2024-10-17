#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ISIZE 5000
#define JSIZE 5001
#define LOC_JSIZE 1667

int main(int argc, char **argv) {
    int i, j, k;
    double *a, *a_1, *a_2;
	double t_start, t_end, working_time;
    int size, rank;
    FILE *ff;

    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		a = (double *) calloc(ISIZE * JSIZE, sizeof(double));
        a_1 = (double *) calloc(ISIZE * LOC_JSIZE, sizeof(double));
		a_2 = (double *) calloc(ISIZE * LOC_JSIZE, sizeof(double));

		for (i = 0; i < ISIZE; i++) {
            for (j = 0, k = 0; j < JSIZE; j += size, k++) {
                *(a + JSIZE * i + j) = 10 * i + j;

				*(a + JSIZE * i + (j + 1)) = 10 * i + (j + 1);
				*(a_1 + LOC_JSIZE * i + k) = *(a + JSIZE * i + (j + 1));

				*(a + JSIZE * i + (j + 2)) = 10 * i + (j + 2);
				*(a_2 + LOC_JSIZE * i + k) = *(a + JSIZE * i + (j + 2));
			}
        }

		MPI_Send((void *) a_1, ISIZE * LOC_JSIZE, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
		MPI_Send((void *) a_2, ISIZE * LOC_JSIZE, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD);

		double t_start = MPI_Wtime();

		for (i = 1; i < ISIZE; i++) {
            for (j = 3; j < JSIZE; j += size) {
                double x = *(a + JSIZE * (i - 1) + (j - 3));
                *(a + JSIZE * i + j) = sin(2 * x);
            }
        }

		MPI_Recv((void *) a_1, ISIZE * LOC_JSIZE, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		
		MPI_Recv((void *) a_2, ISIZE * LOC_JSIZE, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		for (i = 1; i < ISIZE; i++) {
			for (j = 3, k = 1; j < JSIZE && k < LOC_JSIZE; j += size, k++) {
				*(a + JSIZE * i + (j + 1)) = *(a_1 + LOC_JSIZE * i + k);
				*(a + JSIZE * i + (j + 2)) = *(a_2 + LOC_JSIZE * i + k);
			}
		}

		t_end = MPI_Wtime();
		working_time = t_end - t_start;
		printf("%f\n", working_time);

		ff = fopen("p.txt", "w");
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++) {
                fprintf(ff, "%f ", *(a + JSIZE * i + j));
            }
            fprintf(ff, "\n");
        }
        fclose(ff);

		free(a);
		free(a_1);
		free(a_2);
	}

	if (rank == 1) {
		a_1 = (double *) calloc(ISIZE * LOC_JSIZE, sizeof(double));
		MPI_Recv((void *) a_1, ISIZE * LOC_JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		for (i = 1; i < ISIZE; i++) {
        	for (j = 1; j < LOC_JSIZE; j++) {
            	double x = *(a_1 + LOC_JSIZE * (i - 1) + (j - 1));
            	*(a_1 + LOC_JSIZE * i + j) = sin(2 * x);
        	}
    	}

		MPI_Send((void *) a_1, ISIZE * LOC_JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

		free(a_1);
	}

	if (rank == 2) {
		a_2 = (double *) calloc(ISIZE * LOC_JSIZE, sizeof(double));
        MPI_Recv((void *) a_2, ISIZE * LOC_JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
		for (i = 1; i < ISIZE; i++) {
            for (j = 1; j < LOC_JSIZE; j++) {
                double x = *(a_2 + LOC_JSIZE * (i - 1) + (j - 1));
                *(a_2 + LOC_JSIZE * i + j) = sin(2 * x);
            }
        }

        MPI_Send((void *) a_2, ISIZE * LOC_JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        free(a_2);
	}

    MPI_Finalize();

    return 0;
}

