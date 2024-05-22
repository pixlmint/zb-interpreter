#include "zb_headers.h"
#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(length + 1);
    if (buffer) {
        fread(buffer, 1, length, file);
        buffer[length] = '\0';
    }
    fclose(file);

    return buffer;
}

int str_to_int(char* value) {
    int intValue;
    sscanf(value, "%d", &intValue);
    return intValue;
}

