#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *inputFile = fopen("input.txt", "w");
    if (inputFile == NULL) return 1;
    fprintf(inputFile, "2 + 2 * 3\n");
    fclose(inputFile);

    system("python3 evaluate.py");

    FILE *outputFile = fopen("output.txt", "r");
    if (outputFile == NULL) return 1;

    char resultBuffer[256];
    if (fgets(resultBuffer, sizeof(resultBuffer), outputFile) != NULL) {
        printf("%s", resultBuffer);
    }

    fclose(outputFile);
    return 0;
}