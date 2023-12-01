#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <fcntl.h>
#include "sys/wait.h"
#include <sys/stat.h>

int create_process();

int main(int argc, char *argv[]){
    int fd;
    
    char fname[FILENAME_MAX];
    printf("Enter file name: ");
    if(scanf("%255s",fname) != 1){
        perror("Error read filename\n");
        return -1;
    }
    int pipe_fd[2]; // file descriptors for pipe. pipe_fd[0] - for reading, pipe_fd[1] - for writing
    int err = pipe(pipe_fd); //pipe creating
    if (err == -1) // error handling. You can use short form: if (pipe(fd) == -1) and don't create err variable
    {
        perror("pipe");
        return -1;
    }
    pid_t pid = create_process();
    //
    if (0 == pid)
    {
        // child
        close(pipe_fd[0]);
        fd = open(fname, O_RDONLY);
        if(fd == -1){
            close(pipe_fd[1]);
            perror("Error opening file\n");
            return -1;
        }

        // input -> file
        if(dup2(fd, STDIN_FILENO) == -1){
            perror("Error dup2\n");
            return -1;
        }
        close(fd);

        // output -> pipe
        if(dup2(STDOUT_FILENO, pipe_fd[1]) == -1){
            perror("Error dup2\n");
            return -1;
        }

        if(execl("./child", "./child", NULL) == -1){
            perror("exec error");
            return -1;
        }

        close(pipe_fd[1]);
    }
    else
    {
        // parent process
        close(pipe_fd[1]);
        char c;
        int r;
        while ((r = read(pipe_fd[0], &c, 1)) > 0)
        {
            if(r == -1){
                perror("Err read\n");   
            }
            printf("%c", c);
        }
        wait(NULL);
        close(pipe_fd[0]);
    }
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
