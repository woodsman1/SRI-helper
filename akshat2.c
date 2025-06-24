#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_OUTPUT 10000

int main() {
    FILE *infile = fopen("input.txt", "r");
    if (!infile) {
        perror("File open error");
        return 1;
    }

    char line[MAX_LINE];
    char output[MAX_OUTPUT] = "";

    while (fgets(line, sizeof(line), infile)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // Skip empty lines
        if (strlen(line) == 0) continue;

        // Tokenize the line
        char *tokens[100];
        int count = 0;
        char *token = strtok(line, " ");
        while (token) {
            tokens[count++] = token;
            token = strtok(NULL, " ");
        }

        if (count < 3) {
            continue; // skip invalid lines
        }

        // Last two tokens are numbers
        char *num1 = tokens[count - 2];
        char *num2 = tokens[count - 1];

        // Reconstruct name
        char name[MAX_LINE] = "";
        for (int i = 0; i < count - 2; ++i) {
            strcat(name, tokens[i]);
            if (i < count - 3)
                strcat(name, " ");
        }

        // Append to output string
        strcat(output, name);
        strcat(output, ":");
        strcat(output, num1);
        strcat(output, ":");
        strcat(output, num2);
        strcat(output, ";");
    }

    fclose(infile);

    // Final output
    printf("%s\n", output);
    return 0;
}