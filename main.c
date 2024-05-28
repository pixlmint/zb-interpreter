#include <stdio.h>
#include <stdlib.h>
#include "lib/zb_headers.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        char message[100];
        sprintf(message, "Usage: %s <file> <var1> <var2> ...\n", argv[0]);
        e_throw(E_READ_FILE_ERROR, message, -1);
        return 1;
    }

    char* code = read_file(argv[1]);
    if (code == NULL) {
        return 1;
    }

    int inputs[argc - 2];

    for (int i = 2; i < argc; i++) {
        inputs[i - 2] = str_to_int(argv[i]);
    }

    Program* program = create_program_from_config(code, inputs, argc - 2);
    int recursion_depth = 0;

    execute_ast(program->start_node, &recursion_depth);

    printf("x0 = %d\n", program->x0->value);

    free_program(program);
    free(code);

    return 0;
}
