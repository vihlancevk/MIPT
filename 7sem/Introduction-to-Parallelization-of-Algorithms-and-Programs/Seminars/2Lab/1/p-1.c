#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ISIZE 5000
#define JSIZE 5001

int main(int argc, char **argv) {
    int i, j;
    double *a;
    int size, rank;
    int rows_per_proc, start_row, end_row;
    FILE *ff;
    double t_start, t_end, working_time;

    // Инициализация MPI
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Определяем количество строк на каждый процесс
    rows_per_proc = ISIZE / size;
    start_row = rank * rows_per_proc;
    end_row = (rank == size - 1) ? ISIZE : start_row + rows_per_proc;

    // Выделяем память для локальных данных
    a = (double *) calloc(ISIZE * JSIZE, sizeof(double));

    // Инициализация массива на всех процессах
    for (i = 0; i < ISIZE; i++) {
		for (j = 0; j < JSIZE; j++) {
			a[JSIZE*i + j] = 10*i + j;
		}
	}

    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
    	t_start = MPI_Wtime();
	}

    // Вычисления sin для массива a
    for (i = 1; i < ISIZE; i++) {
		for (j = 3; j < JSIZE; j++) {
			double x = a[JSIZE*(i - 1) + (j - 3)];
			a[JSIZE*i + j] = sin(2*x);
		}
	}

    MPI_Barrier(MPI_COMM_WORLD);

    // Сбор данных на корневой процесс
    if (rank == 0) {
    	t_end = MPI_Wtime();
    	working_time = t_end - t_start;
		printf("%f\n", working_time);
    
        // Собираем данные с других процессов
        for (int proc = 1; proc < size; proc++) {
            MPI_Recv(a + proc * rows_per_proc * JSIZE, rows_per_proc * JSIZE, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

		// t_end = MPI_Wtime();
    	// working_time = t_end - t_start;
		// printf("%f\n", working_time);

        // Запись результатов в файл
        ff = fopen("p-1.txt", "w");
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++) {
                fprintf(ff, "%f ", *(a + JSIZE * i + j));
            }
            fprintf(ff, "\n");
        }
        fclose(ff);
    } else {
        // Отправляем данные на корневой процесс
        MPI_Send(a + start_row * JSIZE, rows_per_proc * JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    // Освобождаем память
    free(a);

    // Завершаем работу MPI
    MPI_Finalize();

    return 0;
}

