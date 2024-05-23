#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/zb_headers.h"

void initializeTokensArray(TokenArray* tokens, TokenType* tokenTypes, int countTokenTypes) {
    for (int i = 0; i < countTokenTypes; i++) {
        add_token(tokens, (Token) {tokenTypes[i], NULL});
    }
}

typedef struct {
    int start;
    int maxValues;
    TokenType expectedToken;
    char* description;
} TokenPartTest;

void runGetArrayPartTest(TokenArray* inputArray, TokenPartTest testData) {
    TokenArray* tokensPart = get_array_part(inputArray, testData.start, testData.maxValues);
    if (tokensPart->tokens[0].type != testData.expectedToken) {
        printf("\n%d != %d, dsc: %s\n", testData.expectedToken, tokensPart->tokens[0].type, testData.description);
        CU_FAIL();
    }
    if (tokensPart->size < 1) {
        printf("\narray is too small: %d, dsc: %s\n", tokensPart->size, testData.description);
        CU_FAIL();
    }
    free_token_array(tokensPart);
}

void test_get_array_part() {
    TokenArray* tokens = create_token_array(4);
    TokenType tokenTypes[] = {TOKEN_DO, TOKEN_INT, TOKEN_ANY, TOKEN_SEMICOLON};
    initializeTokensArray(tokens, tokenTypes, 4);
    runGetArrayPartTest(tokens, (TokenPartTest) {0, 1, TOKEN_DO, "token_do"});
    runGetArrayPartTest(tokens, (TokenPartTest) {1, 1, TOKEN_INT, "token_int"});
    runGetArrayPartTest(tokens, (TokenPartTest) {2, 1, TOKEN_ANY, "token_any"});
    free_token_array(tokens);
}

void test_get_array_part_negative() {
    TokenArray* tokens = create_token_array(4);
    TokenType tokenTypes[] = {TOKEN_DO, TOKEN_INT, TOKEN_ANY, TOKEN_SEMICOLON};
    initializeTokensArray(tokens, tokenTypes, 4);
    TokenArray* resultTokens = get_array_part(tokens, 6, 1);
    CU_ASSERT_EQUAL(resultTokens, tokens);
    free_token_array(tokens);
}

void test_dyncamic_array_create() {
    TokenArray* tokens = create_token_array(4); 
    CU_ASSERT_EQUAL(4, tokens->capacity);
    CU_ASSERT_EQUAL(0, tokens->size);
    free_token_array(tokens);
}

void addUnknownTokens(TokenArray* tokens, int tokenCount) {
    for (int i = 0; i < tokenCount; i++) {
        add_token(tokens, (Token){TOKEN_UNKNOWN, NULL});
    }
}

void test_dyn_array_add() {
    TokenArray* tokens = create_token_array(2); 
    addUnknownTokens(tokens, 2);
    CU_ASSERT_EQUAL(2, tokens->capacity);
    CU_ASSERT_EQUAL(2, tokens->size);
    addUnknownTokens(tokens, 1);
    CU_ASSERT_EQUAL(4, tokens->capacity);
    CU_ASSERT_EQUAL(3, tokens->size);
    free_token_array(tokens);
}

void test_verify_next_tokens_equals_assignment_tokens(void) {
    Token tokens[] = {
        {TOKEN_VAR, "x"},
        {TOKEN_ASSIGN, "="},
        {TOKEN_INT, "1"},
        {TOKEN_PLUS, "+"},
        {TOKEN_INT, "2"},
        {TOKEN_SEMICOLON, ";"},
    };
    TokenVerification next_tokens[] = {VERIFY_VAR, VERIFY_ASSIGN, VERIFY_NUMBER, VERIFY_OPERATION, VERIFY_NUMBER, VERIFY_SEMICOLON};
    CU_ASSERT(verify_next_tokens_equals(tokens, next_tokens, 0) == 1);
}

void test_verify_next_tokens_equals_operation_tokens(void) {
    Token tokens[] = {
        {TOKEN_INT, "1"},
        {TOKEN_PLUS, "+"},
        {TOKEN_INT, "2"},
        {TOKEN_SEMICOLON, ";"},
    };
    TokenVerification next_tokens[] = {VERIFY_NUMBER, VERIFY_OPERATION, VERIFY_NUMBER, VERIFY_SEMICOLON};
    CU_ASSERT(verify_next_tokens_equals(tokens, next_tokens, 0) == 1);
}

void test_verify_next_tokens_equals_while_tokens(void) {
    Token tokens[] = {
        {TOKEN_VAR, "x"},
        {TOKEN_GT, ">"},
        {TOKEN_INT, "0"},
        {TOKEN_DO, "do"},
    };
    TokenVerification next_tokens[] = {VERIFY_VAR, VERIFY_GT, VERIFY_INT, VERIFY_DO};
    CU_ASSERT(verify_next_tokens_equals(tokens, next_tokens, 0) == 1);
}

void test_verify_next_tokens_equals_loop_tokens(void) {
    Token tokens[] = {
        {TOKEN_INT, "10"},
        {TOKEN_DO, "do"},
    };
    TokenVerification next_tokens[] = {VERIFY_NUMBER, VERIFY_DO};
    CU_ASSERT(verify_next_tokens_equals(tokens, next_tokens, 0) == 1);
}

void test_find_associated_end_tag_easy() {
    TokenArray* tokens = create_token_array(2);
    TokenType tokenTypes[] = {TOKEN_LOOP, TOKEN_END};
    initializeTokensArray(tokens, tokenTypes, 2);
    int associated_end_tag_index = find_associated_end_tag(tokens, 1);
    CU_ASSERT_EQUAL(1, associated_end_tag_index);
    free_token_array(tokens);
}

void test_find_associated_end_tag_realistic() {
    TokenArray* tokens = create_token_array(10);
    TokenType tokenTypes[] = {TOKEN_LOOP, TOKEN_VAR, TOKEN_DO, TOKEN_VAR, TOKEN_ASSIGN, TOKEN_VAR, TOKEN_PLUS, TOKEN_INT, TOKEN_SEMICOLON, TOKEN_END};
    initializeTokensArray(tokens, tokenTypes, 10);
    int associated_end_tag_index = find_associated_end_tag(tokens, 3);
    CU_ASSERT_EQUAL(9, associated_end_tag_index);
    free_token_array(tokens);
}

void test_find_associated_end_tag_nested() {
    TokenArray* tokens = create_token_array(10);
    TokenType tokenTypes[] = {TOKEN_LOOP, TOKEN_LOOP, TOKEN_END, TOKEN_WHILE, TOKEN_LOOP, TOKEN_END, TOKEN_END, TOKEN_END};
    initializeTokensArray(tokens, tokenTypes, 8);
    int associated_end_tag_index = find_associated_end_tag(tokens, 3);
    CU_ASSERT_EQUAL(7, associated_end_tag_index);
    free_token_array(tokens);
}

void test_create_node_array(void) {
    LinkedASTNodeList* array = create_node_array();

    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_PTR_NULL(array->head);
    CU_ASSERT_PTR_NULL(array->tail);

    free(array);
}

void test_add_node(void) {
    LinkedASTNodeList* array = create_node_array();
    ASTNode* new_node = create_node(NODE_TYPE_UNINIZIALISED);

    add_node(array, new_node);

    CU_ASSERT_PTR_NOT_NULL(array->head);
    CU_ASSERT_PTR_NOT_NULL(array->tail);
    CU_ASSERT_PTR_EQUAL(array->head, new_node);
    CU_ASSERT_PTR_EQUAL(array->tail, new_node);

    free_ast_node(new_node);
    free(array);
}

void suite_tree_builder(CU_pSuite suite) {
    CU_add_test(suite, "test_get_array_part", test_get_array_part);
    CU_add_test(suite, "test_dyncamic_array_create", test_dyncamic_array_create);
    CU_add_test(suite, "test_dyn_array_add", test_dyn_array_add);
    CU_add_test(suite, "test_get_array_part_negative", test_get_array_part_negative);
    CU_add_test(suite, "test_find_associated_end_tag_easy", test_find_associated_end_tag_easy);
    CU_add_test(suite, "test_find_associated_end_tag_realistic", test_find_associated_end_tag_realistic);
    CU_add_test(suite, "test_find_associated_end_tag_nested", test_find_associated_end_tag_nested);
    CU_add_test(suite, "test_create_node_array", test_create_node_array);
    CU_add_test(suite, "test_create_node_array", test_create_node_array);
    CU_add_test(suite, "test_add_node", test_add_node);
    CU_add_test(suite, "test_verify_next_tokens_equals_assignment_tokens", test_verify_next_tokens_equals_assignment_tokens);
    CU_add_test(suite, "test_verify_next_tokens_equals_operation_tokens", test_verify_next_tokens_equals_operation_tokens);
    CU_add_test(suite, "test_verify_next_tokens_equals_while_tokens", test_verify_next_tokens_equals_while_tokens);
    CU_add_test(suite, "test_verify_next_tokens_equals_loop_tokens", test_verify_next_tokens_equals_loop_tokens);
}

