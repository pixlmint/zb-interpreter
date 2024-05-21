#include <stdio.h>
#include <stdlib.h>
#include "lib/tokenizer.h"

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
        fprintf(stderr, "Usage: %s <file> <var1> <var2> ...\n", argv[0]);
        return 1;
    }

    char* code = read_file(argv[1]);
    if (code == NULL) {
        return 1;
    }

    Program* program = init_program();
    for (int i = 2; i < argc; i++) {
        UserVar* user_var = get_variable(i, program);
        user_var->value = str_to_int(argv[i]);
    }

    TokenArray* tokens = tokenize(code);
    ASTNode* start_node = parse_tokens(tokens, program);
    program->start_node = start_node;
    free_token_array(tokens);

    execute_ast(start_node);

    UserVar* x0 = get_variable(0, program);
    printf("x0 = %d\n", x0->value);

    free_program(program);
    free(code);

    return 0;
}
