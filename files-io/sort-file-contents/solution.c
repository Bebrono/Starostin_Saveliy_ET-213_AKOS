#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#define CHUNK_SIZE 1024

void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int compare(const void *a, const void *b) {
    int32_t int_a = *(int32_t *)a;
    int32_t int_b = *(int32_t *)b;
    return (int_a > int_b) - (int_a < int_b);
}

void chunk_sort(int fd, off_t offset, size_t count, int chunk_index) {
    int32_t *buffer = malloc(count * sizeof(int32_t));

    if (!buffer) handle_error("Failed to allocate memory");

    lseek(fd, offset, SEEK_SET);
    if (read(fd, buffer, count * sizeof(int32_t)) != (ssize_t)(count * sizeof(int32_t))) {
        free(buffer);
        handle_error("Failed to read chunk");
    }

    qsort(buffer, count, sizeof(int32_t), compare);

    char temp_fname[64];
    snprintf(temp_fname, sizeof(temp_fname), "temp_chunk_%d.bin", chunk_index);
    int temp_fd = open(temp_fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (temp_fd == -1) {
        free(buffer);
        handle_error("Failed to create temp file");
    }
    if (write(temp_fd, buffer, count * sizeof(int32_t)) != (ssize_t)(count * sizeof(int32_t))) {
        free(buffer);
        handle_error("Failed to write to temp file");
    }
    free(buffer);
    close(temp_fd);
}

void chunk_merge(int num_chunks, const char *output_file) {
    FILE **chunk_files = malloc(num_chunks * sizeof(FILE *));

    if (!chunk_files) handle_error("Failed to allocate memory for chunk files");

    int32_t *buffer = malloc(num_chunks * sizeof(int32_t));
    if (!buffer) handle_error("Failed to allocate memory for buffer");

    for (int i = 0; i < num_chunks; i++) {
        char temp_fname[64];
        snprintf(temp_fname, sizeof(temp_fname), "temp_chunk_%d.bin", i);
        chunk_files[i] = fopen(temp_fname, "rb");

        if (!chunk_files[i]) handle_error("Failed to open temp file");
        if (fread(&buffer[i], sizeof(int32_t), 1, chunk_files[i]) != 1) buffer[i] = INT32_MAX;
    }

    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) handle_error("Failed to create output file");

    while (1) {
        int min_index = -1;
        int32_t min_value = INT32_MAX;

        for (int i = 0; i < num_chunks; i++) {
            if (buffer[i] < min_value) {
                min_value = buffer[i];
                min_index = i;
            }
        }
        if (min_index == -1) break;

        if (write(output_fd, &min_value, sizeof(int32_t)) != sizeof(int32_t)) handle_error("Failed to write output file");

        if (fread(&buffer[min_index], sizeof(int32_t), 1, chunk_files[min_index]) != 1) buffer[min_index] = INT32_MAX;
    }

    for (int i = 0; i < num_chunks; i++) fclose(chunk_files[i]);
    free(chunk_files);
    free(buffer);
    close(output_fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    int fd = open(filename, O_RDONLY);

    if (fd == -1) handle_error("Failed to open file");

    off_t file_size = lseek(fd, 0, SEEK_END);
    size_t total_numbers = file_size / sizeof(int32_t);
    int num_chunks = 0;

    for (off_t offset = 0; offset < file_size; offset += CHUNK_SIZE * sizeof(int32_t)) {
        size_t count = CHUNK_SIZE;

        if (offset + count * sizeof(int32_t) > file_size) count = (file_size - offset) / sizeof(int32_t);
        chunk_sort(fd, offset, count, num_chunks++);
    }

    close(fd);

    chunk_merge(num_chunks, filename);

    for (int i = 0; i < num_chunks; i++) {
        char temp_fname[64];
        snprintf(temp_fname, sizeof(temp_fname), "temp_chunk_%d.bin", i);
        unlink(temp_fname);
    }

    return EXIT_SUCCESS;
}