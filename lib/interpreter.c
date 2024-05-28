#include "zb_headers.h"
#include <stdio.h>

int execute_ast(ASTNode* node, int* depth) {
    char message[100];
    if (*depth >= MAX_DEPTH) {
        sprintf(message, "\nMax depth of %d reached, exiting\n", MAX_DEPTH);
        e_throw(E_RECURSION_ERROR, message, -1);
        return -1;
    }
    int new_val;
    switch (node->type) {
        case NODE_TYPE_ASSIGN:
            *depth += 1;
            new_val = execute_ast(node->data.assign.value, depth);
            *depth -= 1;
            if (new_val < 0) {
                new_val = 0;
            }
            node->data.assign.variable->value = new_val;
            break;
        case NODE_TYPE_WHILE:
            *depth += 1;
            while (execute_ast(node->data.while_loop.condition, depth) != 0) {
                *depth += 1;
                execute_ast(node->data.while_loop.body, depth);
                *depth -= 1;
                node->data.while_loop.iterations++;
            }
            *depth -= 1;
            break;
        case NODE_TYPE_LOOP:
            *depth += 1;
            for (int i = 0; i < execute_ast(node->data.for_loop.count_var, depth); i++) {
                *depth += 1;
                execute_ast(node->data.for_loop.body, depth);
                *depth -= 1;
                node->data.for_loop.iterations++;
            }
            *depth -= 1;
            break;
        case NODE_TYPE_CONSTANT:
            return node->data.constant.value;
        case NODE_TYPE_VARIABLE_ACCESS:
            return node->data.variable_access.variable->value;
        case NODE_TYPE_BINARY_OP:
            switch (node->data.binop.operation) {
                case ADDITION:
                    *depth += 1;
                    int add_retval =  execute_ast(node->data.binop.left, depth) +
                           execute_ast(node->data.binop.right, depth);
                    *depth -= 1;
                    return add_retval;
                case SUBTRACTION:
                    *depth += 1;
                    int sub_retval = execute_ast(node->data.binop.left, depth) -
                            execute_ast(node->data.binop.right, depth);
                    *depth -= 1;
                    return sub_retval;
            }
        default:
            sprintf(message, "undefined behaviour for node type %d\n", node->type);
            e_throw(E_INVALID_TOKEN, message, -1);
    }
    if (node->next) {
        *depth += 1;
        execute_ast(node->next, depth);
        *depth -= 1;
    }
    return -1;
}

