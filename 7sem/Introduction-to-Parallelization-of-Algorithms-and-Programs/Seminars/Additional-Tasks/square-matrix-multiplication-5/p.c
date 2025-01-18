#define CL_TARGET_OPENCL_VERSION 300
#ifdef __APPLE__
    #include <OpenCL/cl.h>
#else
    #include <CL/cl.h>
#endif

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "../square-matrix-multiplication/matrix/matrix.h"

const char* kernelSource = 
"__kernel void mat_mult(__global int* A, __global int* B, __global int* C, const int N) {"
"   int row = get_global_id(0);"
"   int col = get_global_id(1);"
"   int sum = 0.0;"
"   for (int k = 0; k < N; k++) {"
"       sum += A[row * N + k] * B[k * N + col];"
"   }"
"   C[row * N + col] = sum;"
"}";

void custom_free(int* A, int* B, int* C) {
    free(A);
    free(B);
    free(C);
}

int main(int argc, char** argv) {
	// Размер матриц NxN
	int N = 1024;

    // Инициализируем данные
	int* A = read_square_matrix(N, argv[1]);
    if (A == 0) {
        return -1;
    }

    int* B = read_square_matrix(N, argv[2]);
    if (B == 0) {
        return -1;
    }

    int* C = (int*) calloc(N*N, sizeof(int));
    if (C == 0) {
        return -1;
    }

    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue queue = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    cl_mem d_A, d_B, d_C;

    // Получаем платформу и устройство
    clGetPlatformIDs(1, &platform_id, NULL);
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

    // Создаем контекст и очередь команд
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);
    queue = clCreateCommandQueueWithProperties(context, device_id, 0, NULL);

    // Создаем буферы на устройстве
    d_A = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * N * N, NULL, NULL);
    d_B = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * N * N, NULL, NULL);
    d_C = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * N * N, NULL, NULL);

    // Копируем данные на устройство
    clEnqueueWriteBuffer(queue, d_A, CL_TRUE, 0, sizeof(int) * N * N, A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, d_B, CL_TRUE, 0, sizeof(int) * N * N, B, 0, NULL, NULL);

    // Создаем и компилируем программу
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Создаем ядро
    kernel = clCreateKernel(program, "mat_mult", NULL);

    // Устанавливаем аргументы ядра
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_A);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_B);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_C);
    clSetKernelArg(kernel, 3, sizeof(int), &N);

	double start = omp_get_wtime();

    // Задаем размер работы
    size_t globalSize[2] = {N, N};
    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, NULL, 0, NULL, NULL);

    // Копируем результат с устройства
    clEnqueueReadBuffer(queue, d_C, CL_TRUE, 0, sizeof(int) * N * N, C, 0, NULL, NULL);

	double end = omp_get_wtime();
    double time_in_seconds = end - start;
    printf("time: %f\n", time_in_seconds);

    // Выводим результат
    int status = write_square_matrix(N, C, argv[3]);
    if (status != 0) {
        custom_free(A, B, C);
        return -1;
    }

    // Освобождаем ресурсы
	custom_free(A, B, C);
    clReleaseMemObject(d_A);
    clReleaseMemObject(d_B);
    clReleaseMemObject(d_C);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}

