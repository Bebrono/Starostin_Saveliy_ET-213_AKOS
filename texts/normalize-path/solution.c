#include <stddef.h>

void normalize_path(char *path) {
    char *src = path;
    char *dst = path;
    int is_root = (*src == '/');

    if (is_root) {
        *dst++ = *src++;
    }

    while (*src) {
        if (*src == '.' && *(src + 1) == '.' && (*(src + 2) == '/' || *(src + 2) == '\0')) {
            src += (*(src + 2) == '/') ? 3 : 2;

            if (dst > path + is_root) {
                dst--;

                while (dst > path + is_root && *(dst - 1) != '/') {
                    dst--;
                }
            }
            continue;
        }
        if (*src == '.' && (*(src + 1) == '/' || *(src + 1) == '\0')) {
            src += (*(src + 1) == '/') ? 2 : 1;
            continue;
        }
        if (*src == '/' && (*(dst - 1) == '/')) {
            src++;
            continue;
        }
        *dst++ = *src++;
    }

    if (dst > path + is_root && *(dst - 1) == '/') {
        dst--;
    }
    *dst = '\0';
}