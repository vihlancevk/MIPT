#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

void merge(int *arr, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary arrays
	int *L = (int *) calloc(n1, sizeof(int));
	int *R = (int *) calloc(n2, sizeof(int));

    // Copy data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++) {
        L[i] = arr[left + i];
	}
    for (j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
	}

    i = 0;  // Initial index of first subarray
    j = 0;  // Initial index of second subarray
    k = left;  // Initial index of merged subarray

    // Merge the temp arrays back into arr[left..right]
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

	free(L);
	free(R);
}

void merge_sort(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
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
    // printf("Given array is \n");
    // print_array(arr, size);

	double start = omp_get_wtime();

    merge_sort(arr, 0, size - 1);

	double end = omp_get_wtime();
    double time_in_seconds = end - start;
    printf("time: %f\n", time_in_seconds);

    // printf("\nSorted array is \n");
    // print_array(arr, size);

	free(arr);
	return 0;
}
