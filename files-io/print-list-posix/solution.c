#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Item {
    int value;
    uint32_t next_pointer;
};

int main(int argc, char *argv[]) {
    const char *filename = argv[1];
    int fd = open(filename, O_RDONLY);
    struct Item item;
    off_t offset = 0;

    while (1) {
        if (lseek(fd, offset, SEEK_SET) == -1) {
            close(fd);
            return 1;
        }
        ssize_t readBytes = read(fd, &item, sizeof(item));
        if (readBytes == -1) {
            close(fd);
            return 1;

        } else if (readBytes < sizeof(item)) {
            close(fd);
            return 1;
        }
        printf("%d ", item.value);

        if (item.next_pointer == 0) {
            break;
        }
        offset = item.next_pointer;
    }
    close(fd);
    return 0;
}