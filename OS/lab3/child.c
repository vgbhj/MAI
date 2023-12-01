#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <fcntl.h>
#include "sys/wait.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <ctype.h>

int main(int argc, char* argv[]){
    int size = 4096;
    int shm_fd = shm_open(argv[1], O_CREAT | O_RDWR, 0600);
    void* shm_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    int* arr = (int*)shm_ptr;

    int res = 0;
    int is_first_digit = 1;
    int tmp = 0;
    char *p = malloc(size * sizeof(char));
    int is_div_by_zero = 0;
    int r;
    for(int i = 0; i < size; ++i){
        if((r = read(STDIN_FILENO, &p[i], 1) == -1)) perror("Err read\n");
            if(isdigit(p[i])){
                tmp *= 10;
                tmp += p[i] - '0';
                // Debug
                // printf("%d %d \n", res, tmp);
            }
            else if(p[i] == ' '){
                if(tmp == 0){
                    if(is_first_digit == 1){
                        is_first_digit = 0;
                    }
                    else{
                        while (1)
                        {
                            if(arr[0] == 1){
                                arr[1] = -1e9;
                                arr[0] = 0;
                                break;
                                is_div_by_zero = 1;
                            }
                        }

                res = 0;
                tmp = 0;
                is_first_digit = 1;
                        break;
                    }
                }
                else{
                    if(is_first_digit == 1){
                        res = tmp;
                    }
                    else
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
                        while (1)
                        {
                            if(arr[0] == 1){
                                arr[1] = -1e9;
                                arr[0] = 0;
                                break;
                                is_div_by_zero = 1;
                            }
                        }

                res = 0;
                tmp = 0;
                is_first_digit = 1;
                        break;
                    }
                }

                        while (1)
                        {
                            if(arr[0] == 1){
                                arr[1] = res;
                                arr[0] = 0;
                                break;
                            }
                        }
                res = 0;
                tmp = 0;
                is_first_digit = 1;
            }
    }
    if(tmp != 0){
        res /= tmp;

    while (1)
    {
        if(arr[0] == 1){
            arr[1] = res;
            arr[0] = 0;
            break;
        }
    }
    }
    else if(is_div_by_zero != 0){
        if(is_first_digit == 1){
            is_first_digit = 0;
        }
        else{
            while (1)
            {
                if(arr[0] == 1){

                    arr[1] = -1e9;
                    arr[0] = 0;
                    break;
                }
            }
        }
    }

    while(1){
        if(arr[0] == 1){
            arr[0] = 2;
            break;
        }
    }

    munmap(shm_ptr, size);
    close(shm_fd);
    return 0;

}