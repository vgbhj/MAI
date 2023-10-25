#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"
#include <sys/stat.h>
#include <ctype.h>
#include <stdbool.h>

int main(){
    int res = 0;
    int is_first_digit = 1;
    int tmp = 0;
    struct stat v;
    fstat(STDIN_FILENO,&v);//storing all file related information into v
    int size = v.st_size;//st.szie is a member of stat structure and it holds size of file
    //create dynamic array equal to file size 
    char *p = malloc(size * sizeof(char));
    int r;
    for(int i = 0; i < size; ++i){
        if((r = read(STDIN_FILENO, &p[i], 1) == -1)) perror("Err read\n");
            if(isdigit(p[i])){
                if(is_first_digit == 1){
                    res *= 10;
                    res += p[i] - '0';
                }
                else{
                    tmp *= 10;
                    tmp += p[i] - '0';
                }

                // Debug
                // printf("%d %d \n", res, tmp);
            }
            else if(p[i] == ' '){
                if(tmp == 0){
                    if(is_first_digit == 1){
                        is_first_digit = 0;
                    }
                    else{
                        perror("Division by zero!\n");
                        return -1;
                    }
                }
                else{
                    res /= tmp;
                    tmp = 0;
                    is_first_digit = 0;
                }
                
            }
            else if(p[i] == EOF || p[i] == '\n'){
                if(tmp != 0){
                    res /= tmp;
                }
                else{
                    if(is_first_digit == 1){
                        is_first_digit = 0;
                    }
                    else{
                        perror("Division by zero!\n");
                        return -1;
                    }
                }
                printf("%d\n", res);
                res = 0;
                tmp = 0;
                is_first_digit = 1;
            }
            else{
                perror("Invalid character!\n");
                return -1;
            }
    }
    if(tmp != 0){
        res /= tmp;
    }
    else{
        if(is_first_digit == 1){
            is_first_digit = 0;
        }
        else{
            perror("Division by zero!\n");
            return -1;
        }
    }
    printf("%d\n", res);
    return 0;
}