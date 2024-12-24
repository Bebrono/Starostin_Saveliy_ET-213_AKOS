#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_WORD_LEN 4096
#define MAX_WORDS 255

int main() {
    int pipefd[2];
    char word[MAX_WORD_LEN];
    int count = 0;
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;

    } else if (pid == 0) {
        close(pipefd[0]);

        while (scanf("%s", word) == 1) {
            write(pipefd[1], "1", 1);
        }

        close(pipefd[1]);
        exit(0);

    } else {
        close(pipefd[1]);
        char buf[1];

        while (read(pipefd[0], buf, 1) > 0) {
            count++;
        }
        close(pipefd[0]);
        wait(NULL);
        printf("%d\n", count);
    }
    return 0;
}