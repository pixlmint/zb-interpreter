#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"

TokenType OPENING_TAGS[] = {TOKEN_WHILE, TOKEN_LOOP};

TokenArray* tokenize(char* input) {
    TokenArray* tokens = create_token_array(1000);
    int pos = 0;
    int token_count = 0;

    while (input[pos] != '\0') {
        if (isspace(input[pos])) {
            pos++; // Skip whitespace
        } else if (isdigit(input[pos])) {
            // Handle numbers
            int length = 0;
            while (isdigit(input[pos + length])) length++;
            char* number = malloc(length + 1);
            strncpy(number, input + pos, length);
            number[length] = '\0';
            Token intToken = (Token) {TOKEN_INT, number};
            add_token(tokens, intToken);
            // tokens[token_count++] = (Token){TOKEN_INT, number};
            token_count++;
            pos += length;
        } else if (input[pos] == 'x' && isdigit(input[pos + 1])) {
            // Handle variables
            int length = 1;
            while (isdigit(input[pos + length])) length++;
            char* var_name = malloc(length);
            strncpy(var_name, input + pos + 1, length - 1);
            var_name[length - 1] = '\0';
            Token var_token = (Token) {TOKEN_VAR, var_name};
            add_token(tokens, (Token) var_token );
            token_count++;
            pos += length;
        } else {
            Token newToken;
            // Handle single character tokens and keywords
            switch (input[pos]) {
                case '+':
                    newToken = (Token){TOKEN_PLUS, NULL};
                    // tokens[token_count++] = ;
                    break;
                case '-':
                    newToken = (Token){TOKEN_MINUS, NULL};
                    // tokens[token_count++] = (Token){TOKEN_MINUS, NULL};
                    break;
                case '=':
                    newToken = (Token){TOKEN_ASSIGN, NULL};
                    // tokens[token_count++] = (Token){TOKEN_ASSIGN, NULL};
                    break;
                case ';':
                    newToken = (Token){TOKEN_SEMICOLON, NULL};
                    // tokens[token_count++] = (Token){TOKEN_SEMICOLON, NULL};
                    break;
                case '(':
                    newToken = (Token){TOKEN_LPAREN, NULL};
                    // tokens[token_count++] = (Token){TOKEN_LPAREN, NULL};
                    break;
                case ')':
                    newToken = (Token){TOKEN_RPAREN, NULL};
                    // tokens[token_count++] = (Token){TOKEN_RPAREN, NULL};
                    break;
                case '>':
                    newToken = (Token){TOKEN_GT, NULL};
                    // tokens[token_count++] = (Token){TOKEN_GT, NULL};
                    break;
                default:
                    if (strncmp(input + pos, "Loop", 4) == 0) {
                        newToken = (Token){TOKEN_LOOP, NULL};
                        // tokens[token_count++] = (Token){TOKEN_LOOP, NULL};
                        pos += 3; // Account for 'Loop' length -1
                    } else if (strncmp(input + pos, "While", 5) == 0) {
                        newToken = (Token){TOKEN_WHILE, NULL};
                        // tokens[token_count++] = (Token){TOKEN_WHILE, NULL};
                        pos += 4; // Account for 'While' length -1
                    } else if (strncmp(input + pos, "Do", 2) == 0) {
                        newToken = (Token){TOKEN_DO, NULL};
                        // tokens[token_count++] = (Token){TOKEN_DO, NULL};
                        pos += 1; // Account for 'Do' length -1
                    } else if (strncmp(input + pos, "End", 3) == 0) {
                        newToken = (Token){TOKEN_END, NULL};
                        // tokens[token_count++] = (Token){TOKEN_END, NULL};
                        pos += 2; // Account for 'End' length -1
                    } else {
                        newToken = (Token){TOKEN_UNKNOWN, NULL};
                        // tokens[token_count++] = (Token){TOKEN_UNKNOWN, NULL};
                    }
            }
            add_token(tokens, newToken);
            token_count++;
            pos++; // Move past the character
        }
    }
    Token eofToken = (Token){TOKEN_EOF, NULL}; // Mark the end of input
    add_token(tokens, eofToken);
    return tokens;
}

TokenArray* create_token_array(int initial_capacity) {
    TokenArray* array = malloc(sizeof(TokenArray));
    array->tokens = malloc(initial_capacity * sizeof(Token));
    array->size = 0;
    array->capacity = initial_capacity;
    return array;
}

void add_token(TokenArray* array, Token token) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->tokens = realloc(array->tokens, array->capacity * sizeof(Token));
    }
    array->tokens[array->size++] = token;
}

void free_ast_node(ASTNode* node) {
    if (node == NULL) {
        return;
    }

    if (node->type == NODE_TYPE_ASSIGN) {
        if (node->data.assign.value != NULL) {
            free_ast_node(node->data.assign.value);
        }
    } else if (node->type == NODE_TYPE_BINARY_OP) {
        if (node->data.binop.left != NULL) {
            free_ast_node(node->data.binop.left);
        }
        if (node->data.binop.right != NULL) {
            free_ast_node(node->data.binop.right);
        }
    } else if (node->type == NODE_TYPE_LOOP) {
        if (node->data.for_loop.count_var != NULL) {
            free_ast_node(node->data.for_loop.count_var);
        }
        if (node->data.for_loop.body != NULL) {
            free_ast_node(node->data.for_loop.body);
        }
    } else if (node->type == NODE_TYPE_WHILE) {
        if (node->data.while_loop.condition != NULL) {
            free_ast_node(node->data.while_loop.condition);
        }
        if (node->data.while_loop.body != NULL) {
            free_ast_node(node->data.while_loop.body);
        }
    }

    if (node->next != NULL) {
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

ASTNode* create_node(NodeType type) {
    ASTNode* new_node = malloc(sizeof(ASTNode));
    new_node->type = type;
    new_node->next = NULL;
    new_node->data.binop.left = NULL;
    new_node->data.binop.right = NULL;
    new_node->data.assign.value = NULL;
    new_node->data.assign.variable = -1;
    new_node->data.constant.value = -1;
    new_node->data.for_loop.body = NULL;
    new_node->data.for_loop.count_var = NULL;
    new_node->data.while_loop.condition = NULL;
    new_node->data.while_loop.body = NULL;
    new_node->data.variable_access.index = -1;

    return new_node;
}

ASTNode* parse_tokens(TokenArray* tokens) {
    ASTNode* first_node = NULL;
    ASTNode* previous_node = NULL;
    TokenType assignment_tokens[] = {TOKEN_ASSIGN, TOKEN_VAR, TOKEN_ANY, TOKEN_INT, TOKEN_SEMICOLON};
    TokenType operation_tokens[] = {TOKEN_VAR, TOKEN_ANY, TOKEN_INT, TOKEN_SEMICOLON};
    TokenType while_tokens[] = {TOKEN_VAR, TOKEN_GT, TOKEN_INT, TOKEN_DO};
    TokenType loop_tokens[] = {TOKEN_VAR, TOKEN_DO};
    for (int i = 0; i < tokens->size; i++) {
        ASTNode* node;
        Token current_token = tokens->tokens[i];
        switch (current_token.type) {
            case TOKEN_VAR:
                if (verify_next_tokens_equals(tokens->tokens, assignment_tokens, i + 1)) {
                    node = create_node(NODE_TYPE_ASSIGN);
                    node->data.assign.variable = get_variable_index(current_token.value);
                    TokenArray* child_tokens = get_array_part(tokens, i + 2, 4);
                    node->data.assign.value = parse_tokens(child_tokens);
                    // free_token_array(child_tokens);
                    i = i + 4;
                } else if (verify_next_tokens_equals(tokens->tokens, operation_tokens, i)) {
                    node = create_node(NODE_TYPE_BINARY_OP);
                    ASTNode* variable_node = create_node(NODE_TYPE_VARIABLE_ACCESS);
                    ASTNode* constant_node = create_node(NODE_TYPE_CONSTANT);
                    variable_node->data.variable_access.index = get_variable_index(current_token.value);
                    node->data.binop.left = variable_node;
                    constant_node->data.constant.value = get_variable_index(tokens->tokens[i + 2].value);
                    node->data.binop.right = constant_node;
                    switch (tokens->tokens[i + 1].type) {
                        case TOKEN_PLUS:
                            node->data.binop.operation = ADDITION;
                            break;
                        case TOKEN_MINUS:
                            node->data.binop.operation = SUBTRACTION;
                            break;
                        default:
                            fprintf(stderr, "invalid operation\n");
                    }
                    i = i + 3;
                } else {
                    fprintf(stderr, "Expected tokens and actual tokens to not match\n");
                }
                break;
            case TOKEN_LOOP:
                if (verify_next_tokens_equals(tokens->tokens, loop_tokens, i + 1)) {
                    node = create_node(NODE_TYPE_LOOP);
                    ASTNode* count_var = create_node(NODE_TYPE_VARIABLE_ACCESS);
                    count_var->data.variable_access.index = get_variable_index(tokens->tokens[i + 1].value);
                    node->data.for_loop.count_var = count_var;
                    int loop_closing_tag_index = find_associated_end_tag(tokens, i + 2);
                    if (loop_closing_tag_index == -1) {
                        fprintf(stderr, "Unable to find matching closing tag\n");
                    } else {
                        TokenArray* body_tokens = get_array_part(tokens, i + 2, abs(i + 2 - loop_closing_tag_index));
                        node->data.for_loop.body = parse_tokens(body_tokens);
                        // free_token_array(body_tokens);
                        i = loop_closing_tag_index;
                    }
                } else {
                    fprintf(stderr, "Invalid Loop definition\n");
                }
                break;
            case TOKEN_WHILE:
                if (verify_next_tokens_equals(tokens->tokens, while_tokens, i + 1)) {
                    node = create_node(NODE_TYPE_WHILE);
                    ASTNode* condition_var = create_node(NODE_TYPE_VARIABLE_ACCESS);
                    condition_var->data.variable_access.index = get_variable_index(tokens->tokens[i + 1].value);
                    node->data.while_loop.condition = condition_var;
                    int loop_closing_tag_index = find_associated_end_tag(tokens, i + 2);
                    if (loop_closing_tag_index == -1) {
                        fprintf(stderr, "Unable to find matching closing tag");
                    } else {
                        TokenArray* body_tokens = get_array_part(tokens, i + 2, abs(i + 2 - loop_closing_tag_index));
                        node->data.while_loop.body = parse_tokens(body_tokens);
                        // free_token_array(body_tokens);
                        i = loop_closing_tag_index;
                    }
                } else {
                    fprintf(stderr, "Invalid While definition\n");
                }
                break;
            default:
                printf("nothing\n");
        }
        if (first_node == NULL) {
            first_node = node;
        }
        if (previous_node == NULL) {
            previous_node = node;
        } else {
            previous_node->next = node;
        }
    }
    return first_node;
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

TokenArray* get_array_part(TokenArray* tokens, int start, int max_values) {
    int tokens_size =  tokens->size;
    if (tokens_size < start) {
        fprintf(stderr, "Invalid array start value: %d for array of size %d", start, tokens_size);
        return tokens;
    }
    TokenArray* sub_tokens = create_token_array(max_values);
    if (max_values == 1) {
        add_token(sub_tokens, tokens->tokens[start]);
        return sub_tokens;
    }
    int actual_max_values = max_values + start >= tokens_size ? max_values + start : tokens_size;
    for (int i = 0; i < actual_max_values - start; i++) {
        add_token(sub_tokens, tokens->tokens[i + start]);
    }

    return sub_tokens;
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

int get_variable_index(char* value) {
    int intValue;
    sscanf(value, "%d", &intValue);
    return intValue;
}

