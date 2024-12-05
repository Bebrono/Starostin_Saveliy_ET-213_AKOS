#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 4) return 1;

    int fd_input = open(argv[1], O_RDONLY);
    if (fd_input == -1) return 1;

    int fd_digits = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_digits == -1) {
        close(fd_input);
        return 2;
    }

    int fd_others = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_others == -1) {
        close(fd_input);
        close(fd_digits);
        return 2;
    }

    char buffer[4096];
    size_t read_bytes;

    while ((read_bytes = read(fd_input, buffer, sizeof(buffer))) > 0) {
        for (size_t i = 0; i < read_bytes; i++) {

            if (isdigit(buffer[i])) {
                if (write(fd_digits, &buffer[i], 1) == -1) {
                    close(fd_input);
                    close(fd_digits);
                    close(fd_others);
                    return 3;
                }
            } else {
                if (write(fd_others, &buffer[i], 1) == -1) {
                    close(fd_input);
                    close(fd_digits);
                    close(fd_others);
                    return 3;
                }
            }
        }
    }

    if (read_bytes == -1) {
        close(fd_input);
        close(fd_digits);
        close(fd_others);
        return 3;
    }

    close(fd_input);
    close(fd_digits);
    close(fd_others);
    return 0;
}