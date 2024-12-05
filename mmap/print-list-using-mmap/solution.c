#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct Item {
    int value;
    uint32_t next_pointer;
} Item;

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void print_list(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) handle_error("open");

    struct stat st;

    if (fstat(fd, &st) < 0) handle_error("fstat");

    size_t file_size = st.st_size;
    char *data_file = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (data_file == MAP_FAILED) handle_error("mmap");

    uint32_t curr_offset = 0;

    while (curr_offset < file_size) {
        Item *item = (Item *)(data_file + curr_offset);

        printf("%d ", item->value);

        if (item->next_pointer == 0) {
            break;
        }
        curr_offset = item->next_pointer;
    }

    if (munmap(data_file, file_size) < 0) handle_error("munmap");
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    print_list(argv[1]);
    return EXIT_SUCCESS;
}