#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zb_headers.h"

TokenType OPENING_TAGS[] = {TOKEN_WHILE, TOKEN_LOOP};
TokenType ASSIGNMENT_TOKENS[] = {TOKEN_ASSIGN, TOKEN_VAR, TOKEN_ANY, TOKEN_INT, TOKEN_SEMICOLON};
TokenType OPERATION_TOKENS[] = {TOKEN_VAR, TOKEN_ANY, TOKEN_INT, TOKEN_SEMICOLON};
TokenType WHILE_TOKENS[] = {TOKEN_VAR, TOKEN_GT, TOKEN_INT, TOKEN_DO};
TokenType LOOP_TOKENS[] = {TOKEN_VAR, TOKEN_DO};

Program* init_program() {
    Program* program = malloc(sizeof(Program));

    UserVarArray* user_vars = malloc(sizeof(UserVarArray));
    user_vars->size = 0;
    user_vars->capacity = 10;
    user_vars->variables = malloc(sizeof(UserVar) * 10);
    program->variables = user_vars;
    program->x0 = get_variable(0, program);

    return program;
}

Program* create_program_from_config(char* str_program, int* input, int input_size) {
    Program* program = init_program();
    TokenArray* tokens = tokenize(str_program);

    for (int i = 0; i < input_size; i++) {
        UserVar* var = get_variable(i + 1, program);
        var->value = input[i];
    }

    program->start_node = parse_tokens(tokens, program);

    free_token_array(tokens);

    return program;
}

LinkedASTNodeList* create_node_array() {
    LinkedASTNodeList* array = malloc(sizeof(LinkedASTNodeList));
    array->head = NULL;
    array->tail = NULL;
    return array;
}

void add_node(LinkedASTNodeList* array, ASTNode* new_node) {
    if (array->head == NULL) {
        array->head = new_node;
    }
    if (array->tail == NULL) {
        array->tail = new_node;
    } else {
        array->tail->next = new_node;
        array->tail = new_node;
    }
}

ASTNode* create_node(NodeType type) {
    ASTNode* new_node = malloc(sizeof(ASTNode));
    new_node->type = type;
    new_node->next = NULL;

    switch (type) {
        case NODE_TYPE_BINARY_OP:
            new_node->data.binop.left = NULL;
            new_node->data.binop.right = NULL;
            break;
        case NODE_TYPE_ASSIGN:
            new_node->data.assign.value = NULL;
            new_node->data.assign.variable = NULL;
            break;
        case NODE_TYPE_LOOP:
            new_node->data.for_loop.body = NULL;
            new_node->data.for_loop.count_var = NULL;
            new_node->data.for_loop.iterations = 0;
            break;
        case NODE_TYPE_WHILE:
            new_node->data.while_loop.condition = NULL;
            new_node->data.while_loop.body = NULL;
            new_node->data.while_loop.iterations = 0;
            break;
        case NODE_TYPE_CONSTANT:
            new_node->data.constant.value = -1;
            break;
        case NODE_TYPE_VARIABLE_ACCESS:
            break;
        default:
            break;
    }

    return new_node;
}

int handle_assign(TokenArray* tokens, Program* program, ASTNode* node, int* iterator) {
    Token current_token = tokens->tokens[*iterator];
    node->data.assign.variable = get_variable(str_to_int(current_token.value), program);
    TokenArray* child_tokens = get_array_part(tokens, *iterator + 2, 4);
    node->data.assign.value = parse_tokens(child_tokens, program);
    free(child_tokens->tokens);
    free(child_tokens);
    *iterator += 4;
    return 0;
}

int handle_binary_operation(TokenArray* tokens, Program* program, ASTNode* node, int* iterator) {
    Token current_token = tokens->tokens[*iterator];
    ASTNode* variable_node = create_node(NODE_TYPE_VARIABLE_ACCESS);
    ASTNode* constant_node = create_node(NODE_TYPE_CONSTANT);
    variable_node->data.variable_access.variable = get_variable(str_to_int(current_token.value), program);
    node->data.binop.left = variable_node;
    constant_node->data.constant.value = str_to_int(tokens->tokens[*iterator + 2].value);
    node->data.binop.right = constant_node;
    int retval = 0;
    switch (tokens->tokens[*iterator + 1].type) {
        case TOKEN_PLUS:
            node->data.binop.operation = ADDITION;
            break;
        case TOKEN_MINUS:
            node->data.binop.operation = SUBTRACTION;
            break;
        default:
            retval = -1;
            fprintf(stderr, "invalid operation\n");
    }
    *iterator += 3;
    return retval;
}

int handle_for_loop(TokenArray* tokens, Program* program, ASTNode* node, int* iterator) {
    ASTNode* count_var = create_node(NODE_TYPE_VARIABLE_ACCESS);
    count_var->data.variable_access.variable = get_variable(str_to_int(tokens->tokens[*iterator + 1].value), program);
    node->data.for_loop.count_var = count_var;
    int loop_closing_tag_index = find_associated_end_tag(tokens, *iterator + 2);
    if (loop_closing_tag_index == -1) {
        fprintf(stderr, "Unable to find matching closing tag\n");
        return -1;
    } else {
        TokenArray* body_tokens = get_array_part(tokens, *iterator + 3, abs(*iterator + 3 - loop_closing_tag_index));
        node->data.for_loop.body = parse_tokens(body_tokens, program);
        free(body_tokens->tokens);
        free(body_tokens);
        *iterator = loop_closing_tag_index;
    }

    return 0;
}

int handle_while_loop(TokenArray* tokens, Program* program, ASTNode* node, int* iterator) {
    ASTNode* condition_var = create_node(NODE_TYPE_VARIABLE_ACCESS);
    condition_var->data.variable_access.variable = get_variable(str_to_int(tokens->tokens[*iterator + 1].value), program);
    node->data.while_loop.condition = condition_var;
    int loop_closing_tag_index = find_associated_end_tag(tokens, *iterator + 2);

    if (loop_closing_tag_index == -1) {
        fprintf(stderr, "Unable to find matching closing tag");
        return -1;
    } else {
        TokenArray* body_tokens = get_array_part(tokens, *iterator + 5,  loop_closing_tag_index - (*iterator + 5));
        node->data.while_loop.body = parse_tokens(body_tokens, program);
        free(body_tokens->tokens);
        free(body_tokens);
        *iterator = loop_closing_tag_index;
    }

    return 0;
}

ASTNode* parse_tokens(TokenArray* tokens, Program* program) {
    LinkedASTNodeList* nodes = create_node_array();
    for (int i = 0; i < tokens->size; i++) {
        ASTNode* node = NULL;
        Token current_token = tokens->tokens[i];
        switch (current_token.type) {
            case TOKEN_VAR:
                if (verify_next_tokens_equals(tokens->tokens, ASSIGNMENT_TOKENS, i + 1)) {
                    node = create_node(NODE_TYPE_ASSIGN);
                    handle_assign(tokens, program, node, &i);
                } else if (verify_next_tokens_equals(tokens->tokens, OPERATION_TOKENS, i)) {
                    node = create_node(NODE_TYPE_BINARY_OP);
                    handle_binary_operation(tokens, program, node, &i);
                } else {
                    fprintf(stderr, "Expected tokens and actual tokens do not match\n");
                }
                break;
            case TOKEN_LOOP:
                if (verify_next_tokens_equals(tokens->tokens, LOOP_TOKENS, i + 1)) {
                    node = create_node(NODE_TYPE_LOOP);
                    handle_for_loop(tokens, program, node, &i);
                } else {
                    fprintf(stderr, "Invalid Loop definition\n");
                }
                break;
            case TOKEN_WHILE:
                if (verify_next_tokens_equals(tokens->tokens, WHILE_TOKENS, i + 1)) {
                    node = create_node(NODE_TYPE_WHILE);
                    handle_while_loop(tokens, program, node, &i);
                } else {
                    fprintf(stderr, "Invalid While definition\n");
                }
                break;
            case TOKEN_EOF:
            case TOKEN_END:
            case TOKEN_SEMICOLON:
                break;
            default:
                printf("behavior for token %d undefined\n", current_token.type);
        }
        if (node != NULL) {
            add_node(nodes, node);
        }
    }
    ASTNode* head = nodes->head;
    free(nodes);
    return head;
}

int find_associated_end_tag(TokenArray* tokens, int start_position) {
    int opening_tags_seen = 0;
    int opening_tags_count = sizeof(OPENING_TAGS) / sizeof(TokenType);
    for (int i = start_position; i < tokens->size; i++) {
        TokenType current_token = tokens->tokens[i].type;
        for (int x = 0; x < opening_tags_count; x++) {
            if (OPENING_TAGS[x] == current_token) {
                opening_tags_seen += 1;
            }
        }
        if (current_token == TOKEN_END) {
            if (opening_tags_seen == 0) {
                return i;
            } else {
                opening_tags_seen -= 1;
            }
        }
    }

    return -1;
}

int verify_next_tokens_equals(Token* tokens, TokenType* next_tokens, int current_position) {
    int tokens_to_check_size = sizeof(&next_tokens) / sizeof(TokenType);
    for (int i = 0; i < tokens_to_check_size; i++) {
        if (tokens[current_position + i].type != next_tokens[i] && next_tokens[i] != TOKEN_ANY) {
            return 0;
        }
    }
    return 1;
}

