#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void* handle = NULL;
int current_lib = 0;
int (*PrimeCount)(int A, int B);
int* (*Sort)(int* arr, int size);
const char* lib_array[] = {"./lib1.so", "./lib2.so"}; // shared library files
char *error;

int main(int argc, char *argv[]) { 
    if(argc != 3){
        printf("Syntax: ./*executable_file_name* choose library(1/2), choose func(1/2)\n");
        exit(1);
    }

    current_lib = atoi(argv[1])-1;
    if(current_lib != 1 && current_lib != 0){
        printf("Error enter 1 or 2 to args\n");
        exit(1);
    }

    handle = dlopen(lib_array[current_lib], RTLD_LAZY);
    if (!handle) {
        perror("library opening error!" );
        exit(EXIT_FAILURE);
    }
    PrimeCount = (int(*)(int,int))dlsym(handle, "PrimeCount");
    Sort = (int*(*)(int*,int))dlsym(handle, "Sort");

    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }

    if(atoi(argv[2]) == 1){
        int a, b;
        printf("Enter the interval: ");
        scanf("%d %d", &a, &b);
        printf("Count of prime numbers: %d\n", PrimeCount(a,b));
    }
    else if(atoi(argv[2]) == 2){
        int n;
        printf("Enter the length of array: ");
        scanf("%d", &n);
        int* a = (int*)malloc(n * sizeof(int));
        printf("Enter %d numbers of array: ", n);
        for(int i = 0; i < n; i++){
            scanf("%d", &a[i]);
        }
        Sort(a, n);
        printf("\nSorted array: ");
        for(int i = 0; i < n; i++){
            printf("%d ", a[i]);
        }
    }
    dlclose(handle);
}