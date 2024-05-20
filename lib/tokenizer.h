#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "zb_headers.h"

typedef struct {
    Token* tokens;
    int size;
    int capacity;
} TokenArray;

TokenArray* create_token_array(int initial_capacity);
void add_token(TokenArray* array, Token token);

TokenArray* tokenize(char* input);
ASTNode* parse_tokens(TokenArray* tokens);
int parse_statement(Token* current_token, Token* following_tokens, ASTNode** node);
int get_variable_index(char* value);
int verify_next_tokens_equals(Token* tokens, TokenType* next_tokens, int current_position);
TokenArray* get_array_part(TokenArray* tokens, int start, int max_values);
int find_associated_end_tag(TokenArray* tokens, int start_position);
void free_ast_node(ASTNode* node);
void free_token_array(TokenArray* array);

#endif // TOKENIZER_H

