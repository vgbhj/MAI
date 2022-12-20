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


 unsigned int char_to_set(){
    int c;
    unsigned int set = 0b0;
    while (1){
    c = getchar();
    c = ToLower(c);
    if(c == ' ' ){
        continue;
    }
    else if (c == EOF || c == '\n') {
        break;;
    }
    else{
        set = set | (1 << (c - 'a'));
    }
    }
    return set;
 }

int main() {
    int n;
    int count_glasnie = 0;
    int count_soglasnie = 0;
    printf("Enter words ammount: ");
    n = getint();
                           //zyxwvutsrqponmlkjihgfedcba
    unsigned int glasnie = 0b01000100000100000100010001;
                             //zyxwvutsrqponmlkjihgfedcba
    unsigned int soglasnie = 0b10111011111011111011101110;
    unsigned int set_old = char_to_set();
    for (int i = 0; i < n-1; ++i) {
        unsigned int set_new = char_to_set();
        unsigned int res1 = set_old & glasnie;
        unsigned int res2 = set_new & glasnie;
        if ((res1 & res2) != 0) {
            count_glasnie +=1;
        }

        res1 = set_old & soglasnie;
        res2 = set_new & soglasnie;
        if ((res1 & res2) != 0) {
            count_soglasnie +=1;
        }
        set_old = set_new;
    }
    if (count_glasnie != 0) {
        printf("GLASNIE - YES\n");
    }else{
        printf("GLASNIE - N0\n");
    }
    if (count_soglasnie != 0) {
        printf("SOGLASNIE - YES\n");
    }else{
        printf("SOGLASNIE - N0\n");
    }
}