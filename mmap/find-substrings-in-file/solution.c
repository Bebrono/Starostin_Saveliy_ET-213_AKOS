#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void find_substrings(const char *filename, const char *pattern) {
    int fd = open(filename, O_RDONLY);

    if (fd < 0) handle_error("open");

    struct stat st;

    if (fstat(fd, &st) < 0) handle_error("fstat");

    size_t file_size = st.st_size;

    if (file_size == 0) {
        close(fd);
        return;
    }
    char *data_file = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (data_file == MAP_FAILED) handle_error("mmap");

    size_t pattern_len = strlen(pattern);

    if (pattern_len == 0) {
        munmap(data_file, file_size);
        close(fd);
        return;
    }
    for (size_t i = 0; i <= file_size - pattern_len; ++i) {

        if (memcmp(&data_file[i], pattern, pattern_len) == 0) {
            printf("%zu ", i);
        }
    }
    if (munmap(data_file, file_size) < 0) handle_error("munmap");
    close(fd);
}

int main(int argc, char *argv[]) {
    find_substrings(argv[1], argv[2]);
    return EXIT_SUCCESS;
}