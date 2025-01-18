#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define THRESHOLD 32  // Порог для использования сортировки вставками
#define task_size 16384

void insertion_sort(int *arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(int *arr, int *temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

void parallel_merge_sort(int *arr, int *temp, int left, int right) {
    if (right - left <= THRESHOLD) {
        insertion_sort(arr, left, right);
    } else {
        int mid = left + (right - left) / 2;

        #pragma omp task shared(arr, temp) if (right - left > task_size)
        parallel_merge_sort(arr, temp, left, mid);

        #pragma omp task shared(arr, temp) if (right - left > task_size)
        parallel_merge_sort(arr, temp, mid + 1, right);

        #pragma omp taskwait
        merge(arr, temp, left, mid, right);
    }
}

void merge_sort(int *arr, int size) {
    int *temp = (int *) calloc(size, sizeof(int));

    #pragma omp parallel
    {
        #pragma omp single
        parallel_merge_sort(arr, temp, 0, size - 1);
    }

    free(temp);
}

void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int size = 1 << 27;
    int *arr = (int *) calloc(size, sizeof(int));
    
    srand(0);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % size;
    }

	printf("size=%d\n", size);
	printf("task_size=%d\n", task_size);
	// printf("\nSorted array is \n");
    // print_array(arr, size);

    double start = omp_get_wtime();
    
    merge_sort(arr, size);
    
    double end = omp_get_wtime();
    double time_in_seconds = end - start;
    printf("time: %f\n", time_in_seconds);
    
    // printf("\nSorted array is \n");
    // print_array(arr, size);
    
    free(arr);
    return 0;
}

