#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *in_file = fopen("input.txt", "w");
    if (in_file == NULL) {
        return 1;
    }
    fprintf(in_file, "2 + 2 * 3\n");
    fclose(in_file);
    system("python3 evaluate.py");

    FILE *out_file = fopen("output.txt", "r");
    if (out_file == NULL) {
        return 1;
    }
    char res[256];
    if (fgets(res, sizeof(res), out_file) != NULL) {
        printf("%s", res);
    }

    fclose(out_file);
    return 0;
}