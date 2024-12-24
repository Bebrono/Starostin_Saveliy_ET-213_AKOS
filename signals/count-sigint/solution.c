#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t sig_count = 0;

void signal_handler(int signal) {
    if (signal == SIGINT) {
        sig_count++;

    } else if (signal == SIGTERM) {
        printf("%d\n", sig_count);
        fflush(stdout);
        exit(0);
    }
}

int main() {
    printf("%d\n", getpid());
    fflush(stdout);

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    while (1) {
        pause();
    }
    return 0;
}