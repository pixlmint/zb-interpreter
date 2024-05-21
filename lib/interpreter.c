#include "zb_headers.h"
#include <stdio.h>
#include <stdio.h>

int execute_ast(ASTNode* node) {
    switch (node->type) {
        case NODE_TYPE_ASSIGN:
            node->data.assign.variable->value = execute_ast(node->data.assign.value);
            break;
        case NODE_TYPE_WHILE:
            while (execute_ast(node->data.while_loop.condition) != 0) {
                execute_ast(node->data.while_loop.body);
            }
            break;
        case NODE_TYPE_LOOP:
            for (int i = 0; i < execute_ast(node->data.for_loop.count_var); i++) {
                execute_ast(node->data.for_loop.body);
            }
            break;
        case NODE_TYPE_CONSTANT:
            return node->data.constant.value;
        case NODE_TYPE_VARIABLE_ACCESS:
            return node->data.variable_access.variable->value;
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
        default:
            fprintf(stderr, "undefined behaviour for node type %d\n", node->type);
            
    }
    if (node->next) {
        execute_ast(node->next);
    }
    return -1;
}

