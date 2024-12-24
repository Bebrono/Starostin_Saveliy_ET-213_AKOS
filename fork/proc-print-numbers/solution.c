#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void create_processes(int curr, int max) {
    if (curr > max) {
        return;
    }

    pid_t pid = fork();

    if (pid == 0) {

        if (curr > 1) {
            printf(" ");
        }

        printf("%d", curr);
        fflush(stdout);
        create_processes(curr + 1, max);

        if (curr == max) {
            printf("\n");
        }
        exit(0);
    } else {
        wait(NULL);
    }
}

int main(int argc, char *argv[]) {

    int N = atoi(argv[1]);

    if (N <= 0) {
        return 1;
    }
    if (argc != 2) {
        return 1;
    }
    create_processes(1, N);
    return 0;
}