#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char expression[256];

    if (fgets(expression, sizeof(expression), stdin) == NULL) {
        return 1;
    }

    size_t len = strlen(expression);
    if (len > 0 && expression[len - 1] == '\n') {
        expression[len - 1] = '\0';
    }

    if (strlen(expression) == 0) {
        return 1;
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
            expression);
    fclose(tmpFile);

    if (system("gcc temp.c -o temp.out") != 0) {
        return 1;
    }

    system("./temp.out");

    remove("temp.c");
    remove("temp.out");

    return 0;
}