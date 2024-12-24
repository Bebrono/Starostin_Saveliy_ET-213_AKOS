#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char command[256];

    if (fgets(command, sizeof(command), stdin) == NULL) {
        return 1;
    }
    if (strlen(command) == 0) {
        return 1;
    }
    size_t size = strlen(command);

    if (size > 0 && command[size - 1] == '\n') {
        command[size - 1] = '\0';
    }
    FILE *tmpFile = fopen("temp.c", "w");

    if (!tmpFile) {
        return 1;
    }
    fprintf(tmpFile,
            "#include <stdio.h>\n"
            "int main() {\n"
            "    int result = (%s);\n"
            "    printf(\"%%d\\n\", result);\n"
            "    return 0;\n"
            "}\n",
            command);
    fclose(tmpFile);

    if (system("gcc temp.c -o temp.out") != 0) {
        return 1;
    }
    system("./temp.out");

    remove("temp.c");
    remove("temp.out");
    return 0;
}