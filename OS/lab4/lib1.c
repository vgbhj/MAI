#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int PrimeCount(int A, int B){
    int count = 0;
    bool f = false;
    for (int i = A; i <= B; ++i) {
        if(i > 1){
            f = true;
            for (int j = 2; j <= sqrt(i); ++j) {
                if (i % j == 0){
                    f = false;
                }
            }
        }
        if(f){
            ++count;
        }
    }

    return count;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


int *Sort(int* arr, int size){
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }

    return arr;
}