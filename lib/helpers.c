#include "zb_headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        char message[125];
        sprintf(message, "Could not open file %s", filename);
        e_throw(E_READ_FILE_ERROR, message, -1);
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

// Do not define e_throw when running tests
#ifndef TEST_SUITES_H
void e_throw(ErrorCode error, char* message, int line) {
    printf("\n%s\n", message);
    exit(error);
}
#endif

