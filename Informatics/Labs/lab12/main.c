// V 18
#include <ctype.h>
#include <stdio.h>

int main(){
    char c;
    int n = 0;
    int sign = 1;
    while (c != EOF)
    {
        c = getchar();
        if(isdigit(c)){
            n *= 10;
            n += c - '0';
        }
        else{
            int last = n%10;
            n /= 10;
            int sec = n%10;
            n /= 10;
            n *= 10;
            n += last;
            n *= 10;
            n += sec;
            printf("%d \n", n);
            n = 0;
        }
    }
    
    return 0;
}