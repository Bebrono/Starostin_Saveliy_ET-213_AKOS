#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        return 1;
    }
    if (argc != 3) {
        return 1;
    }

    pid_t pid1 = fork();

    if (pid1 < 0) {
        return 1;
    }

    if (pid1 == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        execlp(argv[1], argv[1], (char *)NULL);
        exit(1);
    }
    pid_t pid2 = fork();

    if (pid2 < 0) {
        return 1;
    }
    if (pid2 == 0) {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        execlp(argv[2], argv[2], (char *)NULL);
        exit(1);
    }
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}