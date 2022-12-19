#include <stdio.h>
#include <limits.h>


int main() {
    FILE* file = fopen("test.txt", "r");
    int n;
    fscanf(file, "%d", &n);
    int arr[n+1][n+1];
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
        fscanf(file, "%d", &arr[i][j]);
        }
    }

    int mx = INT_MIN;
    int mn = INT_MAX;
    int min_j;
    int max_i;

    for(int i=1; i<=n; ++i){
        for(int j=1; j<=n; ++j){
            if(arr[i][j] > mx){
                mx = arr[i][j];
                max_i = i;
            }
            if(arr[i][j] < mn){
                mn = arr[i][j];
                min_j = j;
            }
        }
    }

    int sum = 0;
    for(int i=1; i<=n; ++i){
        sum += arr[max_i][i]*arr[i][min_j];
    }

    printf("%d", sum);

}