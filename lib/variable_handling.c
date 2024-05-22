#include "zb_headers.h"
#include <stdlib.h>

int get_variable_index(int variable_key, UserVarArray* vars) {
    for (int i = 0; i < vars->size; i++) {
        if (vars->variables[i].key == variable_key) {
            return i;
        }
    }

    return -1;
}

UserVar* get_variable(int variable_key, Program* program) {
    UserVarArray* vars = program->variables;
    int variable_index = get_variable_index(variable_key, vars);
    if (variable_index == -1) {
        if (vars->size == vars->capacity) {
            vars->capacity *= 2;
            program->variables = realloc(program->variables->variables, program->variables->capacity * sizeof(UserVar));
            vars = program->variables;
        }
        vars->variables[vars->size++] = (UserVar) {variable_key, 0};
        variable_index = vars->size - 1;
    }
    return &vars->variables[variable_index];
}
