#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    fgets(input, MAX_INPUT, stdin);

    // Remove trailing newline
    input[strcspn(input, "\n")] = 0;

    char *tokens[100];
    int count = 0;

    char *token = strtok(input, " ");
    while (token != NULL) {
        tokens[count++] = token;
        token = strtok(NULL, " ");
    }

    if (count < 3) {
        printf("Invalid input format\n");
        return 1;
    }

    char *num1 = tokens[count - 2];
    char *num2 = tokens[count - 1];

    // Reconstruct name
    char name[MAX_INPUT] = "";
    for (int i = 0; i < count - 2; ++i) {
        strcat(name, tokens[i]);
        if (i < count - 3)
            strcat(name, " ");
    }

    printf("Name: %s\n", name);
    printf("Num1: %s\n", num1);
    printf("Num2: %s\n", num2);

    return 0;
}