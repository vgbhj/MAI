#include <stdio.h>
#include <stdlib.h>

// Определения функций из библиотек
extern int PrimeCount();
extern int* Sort();

int main(int argc, char *argv[]) { 
    if(argc != 2){
        printf("Syntax: ./*executable_file_name* function number(1/2)\n");
        exit(1);
    }
    if(atoi(argv[1]) == 1){
        int a, b;
        printf("Enter the interval: ");
        scanf("%d %d", &a, &b);
        printf("Count of prime numbers: %d\n", PrimeCount(a,b));
    }
    else if(atoi(argv[1]) == 2){
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
    return 0;
}
