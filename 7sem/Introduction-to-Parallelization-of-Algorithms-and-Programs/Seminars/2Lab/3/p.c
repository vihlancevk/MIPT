#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ISIZE 5000
#define JSIZE 5000

int main(int argc, char **argv) {
    int i, j;
    double *a, *b;
    int size, rank;
    int rows_per_proc, start_row, end_row;
    FILE *ff;

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
    b = (double *) calloc(ISIZE * JSIZE, sizeof(double));

    // Инициализация массива на всех процессах
    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < JSIZE; j++) {
            *(a + JSIZE * i + j) = 10 * i + j;
            *(b + JSIZE * i + j) = 0;
        }
    }

    // Синхронизация и замер времени
    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    // Вычисления sin для массива a
    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < JSIZE; j++) {
            double x = *(a + JSIZE * i + j);
            *(a + JSIZE * i + j) = sin(0.01 * x);
        }
    }

    // Вычисления для массива b
    for (i = start_row; i < end_row; i++) {
        for (j = 2; j < JSIZE; j++) {
            double x = *(a + JSIZE * i + (j - 2));
            *(b + JSIZE * i + j) = 2.5 * x;
        }
    }

    // Синхронизация и замер времени
    MPI_Barrier(MPI_COMM_WORLD);
    double t_end = MPI_Wtime();
    double working_time = t_end - t_start;

    if (rank == 0) {
        printf("%f\n", working_time);
    }

    // Сбор данных на корневой процесс
    if (rank == 0) {
        // Собираем данные с других процессов
        for (int proc = 1; proc < size; proc++) {
            MPI_Recv(b + proc * rows_per_proc * JSIZE, rows_per_proc * JSIZE, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Запись результатов в файл
        ff = fopen("p.txt", "w");
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++) {
                fprintf(ff, "%f ", *(b + JSIZE * i + j));
            }
            fprintf(ff, "\n");
        }
        fclose(ff);
    } else {
        // Отправляем данные на корневой процесс
        MPI_Send(b + start_row * JSIZE, rows_per_proc * JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    // Освобождаем память
    free(a);
    free(b);

    // Завершаем работу MPI
    MPI_Finalize();

    return 0;
}
