#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct complex {
    double re;
    double im;
};

typedef struct{
    struct complex complex_num;
    int int_num;
}Item;

struct matrix {
    Item v[100];
    int M, N;
    int size;
};

struct matrix Read(FILE *in) {
    struct matrix ans;
    ans.size = 0;
    struct complex c;
    fscanf(in, "%dx%d\n", &ans.M, &ans.N);
    int count = 0;
    for (int i = 0; i < ans.M; i++) {
        ans.v[count].int_num = i+1;
        count++;
        for (int j = 0; j < ans.N; j++) {
            if (fscanf(in, "%lf+%lfi", &c.re, &c.im) == 2) {
                ans.v[count].int_num = j+1;
                count++;
                ans.v[count].complex_num = c;
                count++;
            }
        }
        ans.v[count].int_num = 0;
        count++;
        fscanf(in, "\n");
    }
    ans.size = count;
    return ans;
}

void print_matrix(struct matrix *a) {
    if (!(a->M && a->N)){
        printf("Matrix doesn't exist\n");
        return;
    }
    printf("Matrix size: %dx%d\n", a->M, a->N);
    printf("Internal representation:\n");
    for (int i = 0; i < a->size; i++) {
        if(a->v[i].int_num){
            printf("%d\t", a->v[i].int_num);
        }
        else if(a->v[i].complex_num.im || a->v[i].complex_num.re){
            printf("%.2f+%.2fi\t", a->v[i].complex_num.re, a->v[i].complex_num.im);
        }
        else{
            printf("%d\t", 0);
        }
    }
    printf("\nHuman readable:\n");
    int count = 0;
    struct complex c;
    for (int i = 0; i < a->M; i++) {
        count++;
        for (int j = 0; j < a->N; j++) {
            if(j+1 == a->v[count].int_num){
                count++;
                printf("%.2f+%.2fi\t", a->v[count].complex_num.re, a->v[count].complex_num.im);
                count++;
            }
            else{
                printf("0\t");
            }
        }
        count++;
        printf("\n");
    }
}

void task(struct matrix *a) {
    Item ans;
    int mx_count;
    int count = 0;
    for (int i = 0; i < a->M; i++) {
        int non_null_count = 0;
        Item tmp;
        count++;
        for (int j = 0; j < a->N; j++) {
            if(j+1 == a->v[count].int_num){
                count++;
                non_null_count++;
                tmp.complex_num.re += a->v[count].complex_num.re;
                tmp.complex_num.im += a->v[count].complex_num.im;
                count++;
            }
        }
        tmp.int_num = i+1;
        count++;
        if(non_null_count > mx_count){
            ans = tmp;
            mx_count = non_null_count;
        }
    }
    printf("%d\t", ans.int_num);
    printf("%.2f+%.2fi\t", ans.complex_num.re, ans.complex_num.im);
    printf("\n");
}

int main(int argc, char *argv[]) {
    struct matrix A, B, C;
    int g = 1;
    int c;
    if (argc != 2) {
        printf("Use: program_name input_file\n");
        return 0;
    }
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        perror("Can't open file");
        return 1;
    }
    A = Read(input);
    B = Read(input);
    C.size = 0;
    int choose;
    while (g) {
        printf("1.Print matrix  2. Task  3. Exit\n");
        scanf("%d", &c);
        switch (c) {
            case 1: {
                printf("Choose matrix: 1.A   2.B\n");
                scanf("%d", &choose);
                switch (choose) {
                    case 1: {
                        print_matrix(&A);
                        break;
                    }
                    case 2: {
                        print_matrix(&B);
                        break;
                    }
                    default: {
                        printf("Wrong answer\n");
                        break;
                    }
                }
                break;
            }
            case 2: {
                printf("Choose matrix: 1.A   2.B\n");
                scanf("%d", &choose);
                switch (choose) {
                    case 1: {
                        task(&A);
                        break;
                    }
                    case 2: {
                        task(&B);
                        break;
                    }
                    default: {
                        printf("Wrong answer\n");
                        break;
                    }
                }
                break;
            }
            case 3: {
                g = 0;
                break;
            }
            default: {
                printf("Wrong answer\n");
            }
        }
    }
    return 0;
}