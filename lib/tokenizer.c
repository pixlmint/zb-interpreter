#include "zb_headers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

TokenArray* tokenize(char* input) {
    TokenArray* tokens = create_token_array(10);
    int pos = 0;
    int token_count = 0;
    int current_line = 1;

    while (input[pos] != '\0') {
        if (input[pos] == '\n') {
            pos++;
            current_line++;
        } else if (isspace(input[pos])) {
            pos++; // Skip whitespace
        } else if (isdigit(input[pos])) {
            // Handle numbers
            int length = 0;
            while (isdigit(input[pos + length])) length++;
            char* number = malloc(length + 1);
            strncpy(number, input + pos, length);
            number[length] = '\0';
            Token intToken = (Token) {TOKEN_INT, number, current_line};
            add_token(tokens, intToken);
            token_count++;
            pos += length;
        } else if (input[pos] == 'x' && isdigit(input[pos + 1])) {
            // Handle variables
            int length = 1;
            while (isdigit(input[pos + length])) length++;
            char* var_name = malloc(length);
            strncpy(var_name, input + pos + 1, length - 1);
            var_name[length - 1] = '\0';
            Token var_token = (Token) {TOKEN_VAR, var_name, current_line};
            add_token(tokens, (Token) var_token );
            token_count++;
            pos += length;
        } else if (input[pos] == '/' && input[pos + 1] == '/') {
            while (input[pos] != '\n') {
                pos++;
            }
            current_line++;
        } else {
            Token new_token;
            int skipPositions = 0;
            // Handle single character tokens and keywords
            switch (input[pos]) {
                case '+':
                    new_token = (Token){TOKEN_PLUS, NULL, current_line};
                    break;
                case '-':
                    new_token = (Token){TOKEN_MINUS, NULL, current_line};
                    break;
                case '=':
                    new_token = (Token){TOKEN_ASSIGN, NULL, current_line};
                    break;
                case ';':
                    new_token = (Token){TOKEN_SEMICOLON, NULL, current_line};
                    break;
                case '(':
                    new_token = (Token){TOKEN_LPAREN, NULL, current_line};
                    break;
                case ')':
                    new_token = (Token){TOKEN_RPAREN, NULL, current_line};
                    break;
                case '>':
                    new_token = (Token){TOKEN_GT, NULL, current_line};
                    break;
                default:
                    if (strncmp(input + pos, "Loop", 4) == 0) {
                        new_token = (Token){TOKEN_LOOP, NULL, current_line};
                        skipPositions = 3; // Account for 'Loop' length -1
                    } else if (strncmp(input + pos, "While", 5) == 0) {
                        new_token = (Token){TOKEN_WHILE, NULL, current_line};
                        skipPositions = 4; // Account for 'While' length -1
                    } else if (strncmp(input + pos, "Do", 2) == 0) {
                        new_token = (Token){TOKEN_DO, NULL, current_line};
                        skipPositions = 1; // Account for 'Do' length -1
                    } else if (strncmp(input + pos, "End", 3) == 0) {
                        new_token = (Token){TOKEN_END, NULL, current_line};
                        skipPositions = 2; // Account for 'End' length -1
                    } else {
                        new_token = (Token){TOKEN_UNKNOWN, NULL, current_line};
                    }
            }
            int length = 0;
            while (!isspace(input[pos + length])) length++;
            char* token_string = malloc(length + 1);
            strncpy(token_string, input + pos, length);
            token_string[length] = '\0';
            new_token.value = token_string;
            add_token(tokens, new_token);
            token_count++;
            pos += skipPositions + 1; // Move past the character
        }
    }
    Token eof_token = (Token){TOKEN_EOF, NULL, current_line}; // Mark the end of input
    add_token(tokens, eof_token);
    return tokens;
}

TokenArray* create_token_array(int initial_capacity) {
    TokenArray* array = malloc(sizeof(TokenArray));
    array->tokens = malloc(initial_capacity * sizeof(Token));
    array->size = 0;
    array->capacity = initial_capacity;
    array->contains_invalid_tokens = 0;
    return array;
}

void add_token(TokenArray* array, Token token) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->tokens = realloc(array->tokens, array->capacity * sizeof(Token));
    }
    if (token.type == TOKEN_UNKNOWN) {
        array->contains_invalid_tokens = 1;
    }
    array->tokens[array->size++] = token;
}

TokenArray* get_array_part(TokenArray* tokens, int start, int max_values) {
    int tokens_size =  tokens->size;
    if (tokens_size < start) {
        char message[125];
        sprintf(message, "Invalid array start value: %d, %d", start, tokens_size);
        
        e_throw(E_PARSER_EXCEPTION, message, -1);
        return tokens;
    }
    TokenArray* sub_tokens = create_token_array(max_values);
    if (max_values == 1) {
        add_token(sub_tokens, tokens->tokens[start]);
        return sub_tokens;
    }
    for (int i = 0; i < max_values; i++) {
        if (start + i > tokens_size) {
            break;
        } else {
            add_token(sub_tokens, tokens->tokens[i + start]);
        }
    }

    return sub_tokens;
}

