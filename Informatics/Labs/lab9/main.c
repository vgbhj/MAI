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

int main(){
    int k = 0;
    int i_old=12, j_old=4, l_old=3;
    int i_next, j_next, l_next;
    while (1) {
        i_next = (i_old*j_old/(abs(l_old)+1) + j_old*l_old/(abs(i_old)+1) + i_old*l_old/(abs(j_old)+1))%30;
        j_next = i_old*max(j_old, l_old)%20 + j_old*min(i_old, l_old)%30 - k;
        l_next = max(max(i_old*j_old, i_old*l_old), j_old*l_old)%30 + 20;
        i_old = i_next;
        j_old = j_next;
        l_old = l_next;
        k++;
        if(i_old +j_old <= -10 && i_old+j_old >= -20){
            break;
        }
    }

    printf("Popadanie. Time: %d. End time: %d. i,j=%d,%d k=%d \n", k, k, i_old, j_old, k);
    return 0;
}