#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../lib/zb_headers.h"

void test_tokenize_assign() {
    TokenArray* resultArr = tokenize("=");
    Token* result = resultArr->tokens;
    CU_ASSERT(result[0].type == TOKEN_ASSIGN);
    CU_ASSERT_STRING_EQUAL(result[0].value, "=");
    free_token_array(resultArr);
}

void test_tokenize_parentheses() {
    TokenArray* resultArr = tokenize("( )");
    Token* result = resultArr->tokens;
    CU_ASSERT(result[0].type == TOKEN_LPAREN);
    CU_ASSERT_STRING_EQUAL(result[0].value, "(");
    CU_ASSERT(result[1].type == TOKEN_RPAREN);
    CU_ASSERT_STRING_EQUAL(result[1].value, ")");
    free_token_array(resultArr);
}

void test_tokenize_greater_than() {
    TokenArray* resultArr = tokenize(">");
    Token* result = resultArr->tokens;
    CU_ASSERT(result[0].type == TOKEN_GT);
    CU_ASSERT_STRING_EQUAL(result[0].value, ">");
    free_token_array(resultArr);
}

void test_tokenize_keywords() {
    TokenArray* resultArr = tokenize("Loop While Do End");
    Token* result = resultArr->tokens;
    CU_ASSERT(result[0].type == TOKEN_LOOP);
    CU_ASSERT_STRING_EQUAL(result[0].value, "Loop");
    CU_ASSERT(result[1].type == TOKEN_WHILE);
    CU_ASSERT_STRING_EQUAL(result[1].value, "While");
    CU_ASSERT(result[2].type == TOKEN_DO);
    CU_ASSERT_STRING_EQUAL(result[2].value, "Do");
    CU_ASSERT(result[3].type == TOKEN_END);
    CU_ASSERT_STRING_EQUAL(result[3].value, "End");
    free_token_array(resultArr);
}

void test_tokenize_unknown() {
    TokenArray* resultArr = tokenize("@");
    Token* result = resultArr->tokens;
    CU_ASSERT(result[0].type == TOKEN_UNKNOWN);
    CU_ASSERT_STRING_EQUAL(result[0].value, "@");
    free_token_array(resultArr);
}

void test_tokenize_numbers(void) {
    TokenArray* resultArr = tokenize("123");
    Token* result = resultArr->tokens;
    CU_ASSERT(result[0].type == TOKEN_INT);
    CU_ASSERT_STRING_EQUAL(result[0].value, "123");
    CU_ASSERT(result[1].type == TOKEN_EOF);
    free_token_array(resultArr);
}

void test_tokenize_variables(void) {
    TokenArray* resultArr = tokenize("x1 x2 x123");
    Token* result = resultArr->tokens;
    CU_ASSERT(result[0].type == TOKEN_VAR);
    CU_ASSERT_STRING_EQUAL(result[0].value, "1");
    CU_ASSERT(result[1].type == TOKEN_VAR);
    CU_ASSERT_STRING_EQUAL(result[1].value, "2");
    CU_ASSERT(result[2].type == TOKEN_VAR);
    CU_ASSERT_STRING_EQUAL(result[2].value, "123");
    CU_ASSERT(result[3].type == TOKEN_EOF);
    free_token_array(resultArr);
}

void test_tokenize_operators(void) {
    TokenArray* resultArr = tokenize("+ - ;");
    Token* result = resultArr->tokens;
    CU_ASSERT(result[0].type == TOKEN_PLUS);
    CU_ASSERT(result[1].type == TOKEN_MINUS);
    CU_ASSERT(result[2].type == TOKEN_SEMICOLON);
    CU_ASSERT(result[3].type == TOKEN_EOF);
    free_token_array(resultArr);
}

void test_tokenize_multiline(void) {
    TokenArray* resultArr = tokenize("x1 = x1 + 0;\nx2 = x2 + 0;");
    Token* result = resultArr->tokens;

    CU_ASSERT_EQUAL(result[0].type, TOKEN_VAR);
    CU_ASSERT_EQUAL(result[1].type, TOKEN_ASSIGN);
    CU_ASSERT_EQUAL(result[2].type, TOKEN_VAR);
    CU_ASSERT_EQUAL(result[3].type, TOKEN_PLUS);
    CU_ASSERT_EQUAL(result[4].type, TOKEN_INT);
    CU_ASSERT_EQUAL(result[5].type, TOKEN_SEMICOLON);
    CU_ASSERT_EQUAL(result[6].type, TOKEN_VAR);
    CU_ASSERT_EQUAL(result[7].type, TOKEN_ASSIGN);
    CU_ASSERT_EQUAL(result[8].type, TOKEN_VAR);
    CU_ASSERT_EQUAL(result[9].type, TOKEN_PLUS);
    CU_ASSERT_EQUAL(result[10].type, TOKEN_INT);
    CU_ASSERT_EQUAL(result[11].type, TOKEN_SEMICOLON);

    free_token_array(resultArr);
}

void test_tokenize_loop_realistic() {
    TokenArray* resultArr = tokenize("Loop x1 Do\nx2 = x2 + 1;\nEnd");
    Token* result = resultArr->tokens;

    CU_ASSERT_EQUAL(result[0].type, TOKEN_LOOP);
    CU_ASSERT_EQUAL(result[1].type, TOKEN_VAR);
    CU_ASSERT_STRING_EQUAL(result[1].value, "1");
    CU_ASSERT_EQUAL(result[2].type, TOKEN_DO);
    CU_ASSERT_EQUAL(result[3].type, TOKEN_VAR);
    CU_ASSERT_STRING_EQUAL(result[3].value, "2");
    CU_ASSERT_EQUAL(result[4].type, TOKEN_ASSIGN);
    CU_ASSERT_EQUAL(result[5].type, TOKEN_VAR);
    CU_ASSERT_STRING_EQUAL(result[5].value, "2");
    CU_ASSERT_EQUAL(result[6].type, TOKEN_PLUS);
    CU_ASSERT_EQUAL(result[7].type, TOKEN_INT);
    CU_ASSERT_STRING_EQUAL(result[7].value, "1");
    CU_ASSERT_EQUAL(result[8].type, TOKEN_SEMICOLON);
    CU_ASSERT_EQUAL(result[9].type, TOKEN_END);

    free_token_array(resultArr);
}

void test_tokenize_assign_multi_var() {
    TokenArray* resultArr = tokenize("x1 = x1 - x2;");
    Token* result = resultArr->tokens;

    CU_ASSERT_EQUAL(result[0].type, TOKEN_VAR);
    CU_ASSERT_EQUAL(result[1].type, TOKEN_ASSIGN);
    CU_ASSERT_EQUAL(result[2].type, TOKEN_VAR);
    CU_ASSERT_EQUAL(result[3].type, TOKEN_MINUS);
    CU_ASSERT_EQUAL(result[4].type, TOKEN_VAR);

    free_token_array(resultArr);
}

void test_tokenize_binop_const_then_var() {
    TokenArray* resultArr = tokenize("x1 = 2 - x1;");
    Token* result = resultArr->tokens;

    CU_ASSERT_EQUAL(result[0].type, TOKEN_VAR);
    CU_ASSERT_EQUAL(result[1].type, TOKEN_ASSIGN);
    CU_ASSERT_EQUAL(result[2].type, TOKEN_INT);
    CU_ASSERT_EQUAL(result[3].type, TOKEN_MINUS);
    CU_ASSERT_EQUAL(result[4].type, TOKEN_VAR);

    free_token_array(resultArr);
}

void suite_parser(CU_pSuite suite) {
    CU_add_test(suite, "test_tokenize_numbers", test_tokenize_numbers);
    CU_add_test(suite, "test_tokenize_variables", test_tokenize_variables);
    CU_add_test(suite, "test_tokenize_operators", test_tokenize_operators);
    CU_add_test(suite, "test_tokenize_assign", test_tokenize_assign);
    CU_add_test(suite, "test_tokenize_parentheses", test_tokenize_parentheses);
    CU_add_test(suite, "test_tokenize_greater_than", test_tokenize_greater_than);
    CU_add_test(suite, "test_tokenize_keywords", test_tokenize_keywords);
    CU_add_test(suite, "test_tokenize_unknown", test_tokenize_unknown);
    CU_add_test(suite, "test_tokenize_multiline", test_tokenize_multiline);
    CU_add_test(suite, "test_tokenize_loop_realistic", test_tokenize_loop_realistic);
    CU_add_test(suite, "test_tokenize_assign_multi_var", test_tokenize_assign_multi_var);
    CU_add_test(suite, "test_tokenize_binop_const_then_var", test_tokenize_binop_const_then_var);
}

