#include <sys/syscall.h>
long syscall(long number, ...);

void my_exit_group(int status)
{
    syscall(SYS_exit_group, status);
}

void _start()
{
    char chars[10];
    long readable = 10;
    int over = 1;

    while (over == 1) {
        long previous = readable;
        readable = syscall(SYS_read, 0, chars, 10);
        syscall(SYS_write, 1, chars, readable);

        if (previous != readable) {
            over = 0;
        }
    }

    my_exit_group(0);
}