#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRING_SIZE 100
#define MAX_TEXT_SIZE 100
typedef struct {
    double value;
} key;

typedef struct {
    key k;
    char str[MAX_STRING_SIZE];
} line;

line l[MAX_TEXT_SIZE];
int n = 0;

void print_table() {
    printf("Key\t\t String\n");
    for (int i = 0; i < n; i++) {
        printf("%.1f\t %s", l[i].k.value, l[i].str);
    }
    printf("\n");
}

bool key_equal(key a, key b) {
    return a.value == b.value;
}

bool operator_less(key a, key b) {
    a.value < b.value;
}

void bubble_sort() {
    for(int i = 0; i < n-1; i++){
        for(int j = 0; j < n-1; j++){
            if(l[j].k.value > l[j+1].k.value){
                line tmp = l[j];
                l[j] = l[j+1];
                l[j+1] = tmp;
            }
        }
    }
}

void reverse() {
    for (int i = 0; i < n / 2; i++) {
        line t = l[i];
        l[i] = l[n - 1 - i];
        l[n - 1 - i] = t;
    }
}

void random_table() {
    for (int i = 0; i < n; i++) {
        int j = random() % n;
        line t = l[i];
        l[i] = l[j];
        l[j] = t;
    }
}

void binary_search(key k) {
    int left = 0, right = n, m;
    while (left <= right) {
        m = (left + right) / 2;
        double midVal = l[m].k.value;
        if(midVal == k.value){
            break;
        }
        if(midVal > k.value){
            right = m - 1;
        }
        else{
            left = m + 1;
        }
    }
    // Debug
    // printf("%f, %f",l[m].k.value, k.value );
    if (l[m].k.value == k.value) {
        printf("%.1f\t %s", l[m].k.value, l[m].str);
    } else {
        printf("Key not found\n");
    }
}

int main(int argc, char *argv[]) {
    key k1, k2;
    FILE *f;
    if (argc != 2) {
        printf("Use: program_name input_file\n");
        return 0;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        printf("Can not open file\n");
        return 0;
    }
    while (!feof(f)) {
        // Debug
        fscanf(f, "%lf", &l[n].k.value);
        fgets(l[n].str, MAX_STRING_SIZE, f);
        n++;
    }
    n--;
    int ans, c = 1;
    while (c) {
        printf("1. Print table\t 2. Bubble sort\t 3. Random\t 4. Reverse \t 5. Binary search \t 6. Exit\n");
        scanf("%d", &ans);
        switch (ans) {
            case 1: {
                print_table();
                break;
            }
            case 2: {
                printf("Before:\n");
                print_table();
                bubble_sort();
                printf("After:\n");
                print_table();
                break;
            }
            case 3: {
                random_table();
                break;
            }
            case 4: {
                reverse();
                break;
            }
            case 5: {
                key k;
                printf("Enter key: ");
                scanf("%lf", &k.value);
                binary_search(k);
                break;
            }
            case 6: {
                c = 0;
                break;
            }
            default: {
                printf("Wrong answer\n");
            }
        }
    }
    return 0;
}