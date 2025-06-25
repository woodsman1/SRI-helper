#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ENTRIES 100
#define MAX_NAME_LEN 100

typedef struct {
    char name[MAX_NAME_LEN];
    int value1;
    int value2;
} Entry;

int main() {
    const char *input = "Netflix:7:7; TV VING : 5:5;";
    Entry entries[MAX_ENTRIES];
    int entryCount = 0;

    char buffer[1024];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination

    char *token = strtok(buffer, ";");

    while (token != NULL && entryCount < MAX_ENTRIES) {
        // Trim leading whitespace
        while (*token == ' ') token++;

        // Find first colon to split name
        char *firstColon = strchr(token, ':');
        if (!firstColon) break;

        int nameLen = firstColon - token;
        strncpy(entries[entryCount].name, token, nameLen);
        entries[entryCount].name[nameLen] = '\0';

        // Parse the two integer values after the name
        sscanf(firstColon + 1, "%d:%d", &entries[entryCount].value1, &entries[entryCount].value2);

        entryCount++;
        token = strtok(NULL, ";");
    }

    // Print results
    printf("Parsed Entries:\n");
    for (int i = 0; i < entryCount; i++) {
        printf("Name: '%s', Value1: %d, Value2: %d\n",
               entries[i].name, entries[i].value1, entries[i].value2);
    }

    return 0;
}