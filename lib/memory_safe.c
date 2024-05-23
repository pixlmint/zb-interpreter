#include "zb_headers.h"
#include <stdlib.h>

void free_ast_node(ASTNode* node) {
    if (node == NULL) {
        return;
    }
    switch (node->type) {
        case NODE_TYPE_BINARY_OP:
            free_ast_node(node->data.binop.left);
            free_ast_node(node->data.binop.right);
            break;
        case NODE_TYPE_ASSIGN:
            free_ast_node(node->data.assign.value);
            break;
        case NODE_TYPE_LOOP:
            free_ast_node(node->data.for_loop.count_var);
            free_ast_node(node->data.for_loop.body);
            break;
        case NODE_TYPE_WHILE:
            free_ast_node(node->data.while_loop.condition);
            free_ast_node(node->data.while_loop.body);
            break;
        default:
            break;
    }

    if (node->next != NULL && node->next != node) {
        free_ast_node(node->next);
    }

    free(node);
}

void free_token_array(TokenArray* array) {
    if (array == NULL) {
        return;
    }

    for (int i = 0; i < array->size; i++) {
        if (array->tokens[i].value != NULL) {
            free(array->tokens[i].value);
        }
    }
    if (array->tokens != NULL) {
        free(array->tokens);
    }
    free(array);
}

void free_program(Program* program) {
    free_variables(program->variables);
    free_ast_node(program->start_node);
    free(program->recursion_depth);
    free(program);
}

void free_variables(UserVarArray* user_vars) {
    if (user_vars == NULL) {
        return;
    }
    free(user_vars->variables);
    free(user_vars);
}

