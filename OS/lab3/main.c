#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <fcntl.h>
#include "sys/wait.h"
#include <sys/stat.h>
#include <sys/mman.h>

int create_process();

int main(int argc, char *argv[]){
    char fname[FILENAME_MAX];
    printf("Enter file name: ");
    if(scanf("%4095s",fname) != 1){
        perror("Error read filename\n");
        return -1;
    }

    int size = 4096;
    const char* name = "shared_memory";

    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0600);
    if(shm_fd == -1){
        perror("Error with fd\n");
        return -1;
    }

    if(ftruncate(shm_fd, size) == -1){
        perror("error with trunc\n");
        return -1;
    }

    printf("AAAAAAAAAAAA\n");
    void* shm_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);


    if(shm_ptr == MAP_FAILED){
        perror("Error mapping shared memomy\n");
        munmap(shm_ptr, size);
        close(shm_fd);
        return -1;
    }

    int* arr = (int*)(shm_ptr);
    arr[0] = 1;

    pid_t pid = create_process();

    if (0 == pid)
    {
        // child
        int FD = open(fname, O_RDONLY);
        if(FD == -1){
            munmap(shm_ptr, size);
            close(shm_fd);
            close(FD);
            perror("Error opening file\n");
            return -1;
        }

        // input -> file
        if(dup2(FD, STDIN_FILENO) == -1){

            munmap(shm_ptr, size);
            close(shm_fd);
            close(FD);
            perror("Error dup2\n");
            return -1;
        }
        close(FD);

        if(execl("./child", "./child", name, NULL) == -1){
            perror("exec error");
            return -1;
        }
    }
    else
    {
        // parent process
        while (1)
        {
            if(arr[0] == 2){
                break;
            }
            if(arr[0] == 0){
                if(arr[1] == -1e9){
                    printf("Division by 0\n");
                }
                else{
                    printf("%d\n", arr[1]);
                }

                arr[0] = 1;
            }
        }
        
    }
    munmap(shm_ptr, size);
    close(shm_fd);
    return 0;
}

int create_process() {
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(-1);
    }
    return pid;
}
