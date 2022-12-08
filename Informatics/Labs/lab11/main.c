// V 18
#include <ctype.h>
#include <stdio.h>

void for_alph(int n, char c, int sign){
    if(n != 0){
            if(c == 'C'){
                c = 'F';
                n = n*9/5*sign + 32;
                printf("%d%c", n, c);
                n = 0;
                sign = 1;
            }
        }
        else{
            printf("%c", c);
        }
}

int main(){
    char c;
    int n = 0;
    int sign = 1;
    while (c != EOF)
    {
        c = getchar();
        if(c == '-'){
                sign = -1;
        }
        else if(isdigit(c)){
            n *= 10;
            n += c - '0';
        }
        else if(isalpha(c)){
            for_alph(n, c, sign);
            
        }
        else{
            printf("%c", c);
        }
    }
    
    return 0;
}