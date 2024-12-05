#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main() {
    size_t size = 0;
    struct stat stat_file;
    char name[1024];

    while (fgets(name, sizeof(name), stdin)) {
        size_t len = strlen(name);

        if (len > 0 && name[len - 1] == '\n') {
            name[len - 1] = '\0';
        }
        if (stat(name, &stat_file) == 0) {

            if (S_ISREG(stat_file.st_mode)) {
                size += stat_file.st_size;
            }
        } else {
            perror(name);
        }
    }
    printf("%zu ", size);
    return 0;
}