#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

// Structure to pass data to threads
struct ThreadData {
    int* arr;
    int start;
    int size;
    int dir;
};

// Function to compare two elements based on the direction
void compareAndSwap(int* arr, int i, int j, int dir) {

        // printf("%d %d %d\n", arr[i], arr[j], dir);
    if ((arr[i] > arr[j] && dir == 1) || (arr[i] < arr[j] && dir == 0)) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Function to perform bitonic merge
void bitonicMerge(int* arr, int start, int size, int dir) {
    if (size > 1) {
        int k = size / 2;
        for (int i = start; i < start + k; ++i) {
            compareAndSwap(arr, i, i + k, dir);
        }

        // Recursively perform bitonic merge on both halves
        bitonicMerge(arr, start, k, dir);
        bitonicMerge(arr, start + k, k, dir);
    }
}

// Function to perform bitonic sort
void bitonicSort(int* arr, int start, int size, int dir) {
    if (size > 1) {
        int k = size / 2;

        // Recursively sort the two halves
        bitonicSort(arr, start, k, 1);
        bitonicSort(arr, start + k, k, 0);

        // Perform bitonic merge
        bitonicMerge(arr, start, size, dir);
    }
}

// Thread function to perform bitonic sort on a portion of the array
void* threadBitonicSort(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    bitonicSort(data->arr, data->start, data->size, data->dir);
    return NULL;
}

bool isPowerOfTwo(int x)
{
    /* First x in the below expression is for the case when x is 0 */
    return x && (!(x & (x - 1)));
}


int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Syntax: ./*executable_file_name* Size_of_array Max_number_of_threads\n");
        exit(1);
    }
    srand(time(NULL));
    int size = atoi(argv[1]);
    if(!isPowerOfTwo(size)){
        printf("Size must be power of 2\n");
        exit(1);
    }
    int* arr = (int*)malloc(size * sizeof(int));

    printf("Original Array:\n");
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 100;
        printf("%d ", arr[i]);
    }
    
    printf("\n");


    // Record start time
    clock_t start_time = clock();

    // Create N threads
    int numThreads = atoi(argv[2]);
    if(!isPowerOfTwo(numThreads)){
        printf("Number of threads must be power of 2\n");
        exit(1);
    }
    if(numThreads > size){
        numThreads = size;
    }
    pthread_t threads[numThreads];
    struct ThreadData threadData[numThreads];

    int chunkSize = size / numThreads;

    int status;
    int status_addr;
    // Divide the array into chunks and assign each chunk to a thread
    for (int i = 0; i < numThreads; ++i) {
        threadData[i].arr = arr;
        threadData[i].start = i * chunkSize;
        threadData[i].size = chunkSize;
        threadData[i].dir = (i+1)%2;
        status = pthread_create(&threads[i], NULL, threadBitonicSort, (void*)&threadData[i]);
        if (status != 0) {
            printf("main error: can't create thread, status = %d\n", status);
            exit(ERROR_CREATE_THREAD);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < numThreads; ++i) {
        status = pthread_join(threads[i], (void**)&status_addr);
        if (status != SUCCESS) {
            printf("main error: can't join thread, status = %d\n", status);
            exit(ERROR_JOIN_THREAD);
        }
    }
    

    // Record end time
    clock_t end_time = clock();

    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", execution_time);
    
    printf("\nPreSorted Array:\n");
    for (int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    for(int i = 1; i <= (int)log2(size/chunkSize); ++i){
        for(int j = 0 ; j < size/chunkSize/(int)pow(2,i); ++j){
            bitonicMerge(arr, chunkSize*(int)pow(2,i)*j, chunkSize*(int)pow(2,i), (j+1)%2);
        }
    }


    printf("\nSorted Array:\n");
    for (int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Clean up
    free(arr);

    return 0;
}
