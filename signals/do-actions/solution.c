#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t count = 0;

void signal_handler(int signal) {
    if (signal == SIGUSR1) {
        count += 1;
        printf("%d\n", count);
        fflush(stdout);

    } else if (signal == SIGUSR2) {
        count *= -1;
        printf("%d\n", count);
        fflush(stdout);

    } else if (signal == SIGTERM || signal == SIGINT) {
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;

    count = atoi(argv[1]);
    printf("%d\n", getpid());
    fflush(stdout);

    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    while (1) {
        pause();
    }
    return 0;
}