// V 18

#include <stdlib.h>
#include <stdio.h>

int min(int a, int b){
    if(a<=b){
        return a;
    }
    return b;
}

int max(int a,int b){
    if(a>=b){
        return a;
    }
    return b;
}

int mod(int a, int b){
    // printf("%d \n", b);
    return a - a/b*b;
    // return a%b;
}

int main(){
    int k = 0;
    int i_old=-22, j_old=14, l_old=-14;
    int i_next, j_next, l_next;
    while (1) {
        if(k != 10){
        i_next = mod((i_old*min(j_old, l_old)+j_old*min(i_old,l_old)+k*k),20);
        j_next = mod((mod(i_old, 10-k)*mod(j_old, 10+k)*mod(l_old, 10-k)),25);
        // j_next = (mod(i_old, 10-k)*mod(j_old, 10+k)*mod(mod(l_old, 10-k),25));
        l_next = max(mod(min(i_old+j_old, i_old+l_old),25), mod(max(i_old+l_old, j_old+k),20))+10;
        i_old = i_next;
        j_old = j_next;
        l_old = l_next;
        if(i_old +j_old <= -10 && i_old+j_old >= -20){
            break;
        }
        }
        k++;
        printf("Popadanie. Time: %d. End time: %d. i,j=%d,%d k=%d \n", k, k, i_old, j_old, k);

    }

    printf("Popadanie. Time: %d. End time: %d. i,j=%d,%d k=%d \n", k, k, i_old, j_old, k);
    return 0;
}