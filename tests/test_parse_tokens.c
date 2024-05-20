#include <CUnit/Basic.h>
#include "../lib/tokenizer.h"

void test_parse_basic_tokens() {
    char* str_program = "x1 = x1 + 0;";
    TokenArray* tokens = tokenize(str_program);
    ASTNode* program = parse_tokens(tokens);

    CU_ASSERT_EQUAL(program->type, NODE_TYPE_ASSIGN);
    CU_ASSERT_EQUAL(program->data.assign.variable, 1);
    ASTNode* assign_node = program->data.assign.value;
    CU_ASSERT_EQUAL(assign_node->type, NODE_TYPE_BINARY_OP);
    CU_ASSERT_EQUAL(assign_node->data.binop.left->data.variable_access.index, 1);
    CU_ASSERT_EQUAL(assign_node->data.binop.operation, ADDITION);
    CU_ASSERT_EQUAL(assign_node->data.binop.right->data.constant.value, 0);

    free_token_array(tokens);
    // free_ast_node(program);
}

void test_parse_multi_tokens() {
    char* str_program = "x1 = x1 + 0;x2 = x2 + 0;";
    TokenArray* tokens = tokenize(str_program);
    ASTNode* program = parse_tokens(tokens);

    CU_ASSERT_EQUAL(program->type, NODE_TYPE_ASSIGN);
    CU_ASSERT_EQUAL(program->data.assign.variable, 1);
    ASTNode* assign_node = program->data.assign.value;
    CU_ASSERT_EQUAL(assign_node->type, NODE_TYPE_BINARY_OP);
    CU_ASSERT_EQUAL(assign_node->data.binop.left->data.variable_access.index, 1);
    CU_ASSERT_EQUAL(assign_node->data.binop.operation, ADDITION);
    CU_ASSERT_EQUAL(assign_node->data.binop.right->data.constant.value, 0);
    CU_ASSERT_NOT_EQUAL(program->next, NULL);
    ASTNode* next_program = program->next;
    if (next_program->type == NODE_TYPE_ASSIGN) {
        CU_ASSERT_EQUAL(next_program->data.assign.variable, 2);
        ASTNode* next_assign_node = next_program->data.assign.value;
        CU_ASSERT_EQUAL(next_assign_node->type, NODE_TYPE_BINARY_OP);
        CU_ASSERT_EQUAL(next_assign_node->data.binop.left->data.variable_access.index, 2);
        CU_ASSERT_EQUAL(next_assign_node->data.binop.operation, ADDITION);
        CU_ASSERT_EQUAL(next_assign_node->data.binop.right->data.constant.value, 0);
    } else {
        CU_ASSERT_EQUAL(next_program->type, NODE_TYPE_ASSIGN);
        printf("\nwrong node type: %d\n", next_program->type);
    }

    // free_ast_node(program);
    free_token_array(tokens);
}

void suite_parse_tokens(CU_pSuite suite) {
    CU_add_test(suite, "test_parse_basic_tokens", test_parse_basic_tokens);
    CU_add_test(suite, "test_parse_multi_tokens", test_parse_multi_tokens);
}
