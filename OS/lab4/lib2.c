#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

int PrimeCount(int A, int B){
    int count = 0;
    bool *is_prime = malloc((B + 1) * sizeof(bool));

    // Инициализация массива
    for (int i = 0; i <= B; ++i) {
        is_prime[i] = true;
    }

    // Маркировка составных чисел
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= sqrt(B); ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= B; j += i) {
                is_prime[j] = false;
            }
        }
    }

    // Подсчет простых чисел на отрезке [A, B]
    for (int i = A; i <= B; ++i) {
        if (is_prime[i]) {
            ++count;
        }
    }

    free(is_prime);

    return count;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Функция для разделения массива на две части и возврата индекса опорного элемента
int partition(int* arr, int low, int high) {
    int pivot = arr[low];
    int left = low + 1;
    int right = high;

    while (1) {
        while (left <= right && arr[left] < pivot) {
            left++;
        }

        while (right >= left && arr[right] > pivot) {
            right--;
        }

        if (left <= right) {
            swap(&arr[left], &arr[right]);
        } else {
            break;
        }
    }

    swap(&arr[low], &arr[right]);
    return right;
}

// Функция быстрой сортировки (Хоара)
int* quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }

    return arr;
}

int* Sort(int* arr, int size){
    quickSort(arr, 0, size-1);
    return arr;
}