#include <stdio.h>
#include <stdlib.h>
#include "lib/zb_headers.h"

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    char* code = read_file(argv[1]);
    if (code == NULL) {
        return 1;
    }

    Program* program = create_program_from_config(code);

    execute_ast(program->start_node);

    free_program(program);
    free(code);

    return 0;
}
