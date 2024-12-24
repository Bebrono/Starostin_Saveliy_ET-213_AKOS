#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE sizeof(BlockHeader)

typedef struct BlockHeader {
    size_t size;
    int free;
    struct BlockHeader *next;
} BlockHeader;

static int fd_swap = -1;
static void *start_swap = NULL;
static size_t size_swap = 0;
static BlockHeader *list_free = NULL;

extern void myalloc_initialize(int fd) {
    fd_swap = fd;
    size_swap = lseek(fd, 0, SEEK_END);

    start_swap = mmap(NULL, size_swap, PROT_READ | PROT_WRITE, MAP_SHARED, fd_swap, 0);

    list_free = (BlockHeader *)start_swap;
    list_free->size = size_swap - BLOCK_SIZE;
    list_free->free = 1;
    list_free->next = NULL;
}

extern void myalloc_finalize() {
    start_swap = NULL;
    fd_swap = -1;
    size_swap = 0;
    list_free = NULL;
}

extern void *my_malloc(size_t size) {
    BlockHeader *current = list_free;
    BlockHeader *prev = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            size_t remaining_size = current->size - size - BLOCK_SIZE;

            if (remaining_size >= BLOCK_SIZE) {
                BlockHeader *new_block = (BlockHeader *)((char *)current + BLOCK_SIZE + size);
                new_block->size = remaining_size;
                new_block->free = 1;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            current->free = 0;
            return (void *)((char *)current + BLOCK_SIZE);
        }

        prev = current;
        current = current->next;
    }

    return NULL;
}

extern void my_free(void *ptr) {
    BlockHeader *block = (BlockHeader *)((char *)ptr - BLOCK_SIZE);
    block->free = 1;

    BlockHeader *current = list_free;
    while (current != NULL) {
        if (current->free && current->next && current->next->free) {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}