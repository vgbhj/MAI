#include <stdio.h>

struct pair{
    int x;
    int y;
};

bool at_the_border(int next_x, int next_y, int n){
    return (next_x > 0 && next_x < n+1) && (next_y > 0 && next_y < n+1);
}

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

    struct pair pos;
    pos.x = n/2+n%2;
    pos.y = n/2+n%2;

    // Debug
    // printf("%d %d \n", pos.x, pos.y);
    int side = 3;
    int old_x = pos.x;
    int old_y = pos.y;
    while(at_the_border(pos.x, pos.y, n)){
        printf("%d ", arr[pos.y][pos.x]);
        pos.x++;
        // to_bot
        while(at_the_border(pos.x, pos.y, n) && pos.y < old_y+(side-2)){   
            printf("%d ", arr[pos.y][pos.x]);
            pos.y++;
        }

        old_x = pos.x;
        old_y = pos.y;
        // to_right
        while(at_the_border(pos.x, pos.y, n) && pos.x > old_x-(side)+1){   
            printf("%d ", arr[pos.y][pos.x]);
            pos.x--;
        }

        old_x = pos.x;
        old_y = pos.y;

        // to_top
        while(at_the_border(pos.x, pos.y, n) && pos.y > old_y-side+1){ 
            printf("%d ", arr[pos.y][pos.x]);
            pos.y--;
        }

        old_x = pos.x;
        old_y = pos.y;

        // to_left
        while(at_the_border(pos.x, pos.y, n) && pos.x < old_x+side-1){ 
            printf("%d ", arr[pos.y][pos.x]);
            pos.x++;
        }
        side += 2;
    }
}