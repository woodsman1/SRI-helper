#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int main() {
    FILE *infile = fopen("input.txt", "r");
    FILE *outfile = fopen("output.txt", "w");

    if (!infile || !outfile) {
        perror("File open error");
        return 1;
    }

    char line[MAX_LINE];
    int first = 1;  // To avoid leading semicolon

    while (fgets(line, sizeof(line), infile)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        char *tokens[100];
        int count = 0;

        char *token = strtok(line, " ");
        while (token) {
            tokens[count++] = token;
            token = strtok(NULL, " ");
        }

        if (count < 3) {
            continue;  // skip invalid lines
        }

        char *num1 = tokens[count - 2];
        char *num2 = tokens[count - 1];

        char name[MAX_LINE] = "";
        for (int i = 0; i < count - 2; i++) {
            strcat(name, tokens[i]);
            if (i < count - 3) strcat(name, " ");
        }

        fprintf(outfile, "%s%s:%s:%s;", first ? "" : "", name, num1, num2);
        first = 0;
    }

    fclose(infile);
    fclose(outfile);
    return 0;
}