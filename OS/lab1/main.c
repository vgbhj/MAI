#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <fcntl.h>
#include "sys/wait.h"
#include <sys/stat.h>
                                                         
#define  MAX_LEN  100  

int create_process();

int main(){
    int fd;
    char fname[128];
    printf("Enter file name: ");
    scanf("%s",fname);
    int pipe_fd[2]; // file descriptors for pipe. pipe_fd[0] - for reading, pipe_fd[1] - for writing
    int err = pipe(pipe_fd); //pipe creating
    if (err == -1) // error handling. You can use short form: if (pipe(fd) == -1) and don't create err variable
    {
        perror("pipe");
        return -1;
    }
    pid_t pid = create_process();
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
        struct stat v;
        fstat(pipe_fd[0],&v);//storing all file related information into v
        int size = v.st_size;//st.szie is a member of stat structure and it holds size of file
        //create dynamic array equal to file size 
        char *p = malloc(size * sizeof(char));
        // printf("%d ", size);
        int r;
        for(int i = 0; i < size; ++i){
            if((r = read(pipe_fd[0], &p[i], 1) == -1)) perror("Err read\n");   
            printf("%c ", p[i]);
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
