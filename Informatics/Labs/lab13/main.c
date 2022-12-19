#include <stdio.h>


int ToLower(int c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; 
}

int getint(){
    int ch = 0;
    while(1){
        int c = getchar();
        c = ToLower(c);
        if (c == ' ' || c == '\n' || c == EOF){
            break;
        }else{
            ch = ch*10 + (c-'0');
        }

    }
    return ch;
}


 unsigned int getsetword(){
    int c;
    unsigned int set = 0b0;
    while (1){
    c = getchar();
    c = ToLower(c);
    if (c == ' ' || c == EOF || c == '\n') {
        break;
    }
    else{
        set = set | (1 << (c - 'a'));
    }
    }
    return set;
 }

int main() {
    int n;
    int count = 0;
    printf("Enter words ammount: ");
    n = getint();
    unsigned int glasnieonly = 0b01000100000100000100010001;
    unsigned int setpred = getsetword();
    for (int i = 0; i < n-1; ++i) {
        unsigned int setnew = getsetword();
        unsigned int res1 = setpred & glasnieonly;
        unsigned int res2 = setnew & glasnieonly;
        //printf("%d %d\n", setpred, setnew);
        //printf("%d %d\n", res1, res2);
        if ((res1 & res2) != 0) {
            count +=1;
        }
        setpred = setnew;
    }
    if (count != 0) {
        printf("Yes, thes words exist, the ammount of pairs: %d", count);
    }else{
        printf("No, words letters combinations dont match");
    }
}