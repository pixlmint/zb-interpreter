#include "zb_headers.h"
#include <stdio.h>

/*int execute_ast(ASTNode* node) {
    int variables[MAX_VARS];
    switch (node->type) {
        case NODE_TYPE_ASSIGN:
            variables[node->data.assign.variable] = evaluate_expression(node->data.assign.value);
            break;
        case NODE_TYPE_WHILE:
            while (evaluate_expression(node->data.loop.condition)) {
                execute_ast(node->data.loop.body);
            }
            break;
        case NODE_TYPE_LOOP:
            // Similar logic
            break;
        case NODE_TYPE_EXPR:
            // Evaluate expressions
            break;
        case NODE_TYPE_CONSTANT:
            return node->data.constant.value;
        case NODE_TYPE_VARIABLE_ACCESS:
            return variables[node->data.variable_access.index];
        case NODE_TYPE_BINARY_OP:
            switch (node->data.binop.operation) {
                case ADDITION:
                    return execute_ast(node->data.binop.left) +
                           execute_ast(node->data.binop.right);
                case SUBTRACTION:
                    return execute_ast(node->data.binop.left) -
                            execute_ast(node->data.binop.right);
                default:
                    fprintf(stderr, "Unsupported operation\n");
                    return 0;
            }
    }
    if (node->next) {
        execute_ast(node->next);
    }
    return -1;
}*/

