#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    char* in = argv[2];
    int pipefd[2];
    char* cmd = argv[1];
    
    if (argc != 3) {
        return 1;
    }
    pipe(pipefd);

    pid_t pid = fork();

    if (pid > 0) {
        int wstatus = 0;
        waitpid(pid, &wstatus, 0);
        close(pipefd[1]);

        char buf[4096];
        size_t total_bytes = 0;
        ssize_t read_bytes;
    
        while ((read_bytes = read(pipefd[0], buf, sizeof(buf))) > 0) {
            total_bytes += read_bytes;
        }
        close(pipefd[0]);
        printf("%zu\n", total_bytes);

    } else if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        int input_fd = open(in, O_RDONLY);

        if (input_fd < 0) {
            exit(1);
        }

        dup2(input_fd, STDIN_FILENO);
        close(input_fd);

        execlp(cmd, cmd, NULL);
        exit(1);

    } else {
        return 1;
    }
    return 0;
}