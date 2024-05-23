#include <CUnit/Basic.h>
#include "../lib/zb_headers.h"

void test_parse_basic_tokens() {
    char* str_program = "x1 = x1 + 0;";
    Program* actual_program = init_program();
    TokenArray* tokens = tokenize(str_program);
    ASTNode* program = parse_tokens(tokens, actual_program);
    free_token_array(tokens);
    actual_program->start_node = program;

    CU_ASSERT_EQUAL(program->type, NODE_TYPE_ASSIGN);
    CU_ASSERT_EQUAL(program->data.assign.variable->key, 1);
    ASTNode* assign_node = program->data.assign.value;
    CU_ASSERT_EQUAL(assign_node->type, NODE_TYPE_BINARY_OP);
    CU_ASSERT_EQUAL(assign_node->data.binop.left->data.variable_access.variable->key, 1);
    CU_ASSERT_EQUAL(assign_node->data.binop.operation, ADDITION);
    CU_ASSERT_EQUAL(assign_node->data.binop.right->data.constant.value, 0);

    free_program(actual_program);
}

void test_parse_tokens_assign_multi_variables() {
    char *str_program = "x1 = x1 + x2;";
    Program* program = create_program_from_config(str_program, NULL, 0);
    ASTNode* root = program->start_node;

    CU_ASSERT_EQUAL(root->type, NODE_TYPE_ASSIGN);
    CU_ASSERT_EQUAL(root->data.assign.variable->key, 1);
    ASTNode* assign_node = root->data.assign.value;
    CU_ASSERT_EQUAL(assign_node->type, NODE_TYPE_BINARY_OP);
    CU_ASSERT_EQUAL(assign_node->data.binop.left->type, NODE_TYPE_VARIABLE_ACCESS);
    CU_ASSERT_EQUAL(assign_node->data.binop.right->type, NODE_TYPE_VARIABLE_ACCESS);
    CU_ASSERT_EQUAL(assign_node->data.binop.left->data.variable_access.variable->key, 1);
    CU_ASSERT_EQUAL(assign_node->data.binop.operation, ADDITION);
    CU_ASSERT_EQUAL(assign_node->data.binop.right->data.variable_access.variable->key, 2);
}

void test_parse_multi_tokens() {
    char* str_program = "x1 = x1 + 0;x2 = x2 + 0;";
    Program* program = create_program_from_config(str_program, (int[]){}, 0);
    ASTNode* root = program->start_node;

    CU_ASSERT_EQUAL(root->type, NODE_TYPE_ASSIGN);
    CU_ASSERT_EQUAL(root->data.assign.variable->key, 1);
    ASTNode* assign_node = root->data.assign.value;
    CU_ASSERT_EQUAL(assign_node->type, NODE_TYPE_BINARY_OP);
    CU_ASSERT_EQUAL(assign_node->data.binop.left->data.variable_access.variable->key, 1);
    CU_ASSERT_EQUAL(assign_node->data.binop.operation, ADDITION);
    CU_ASSERT_EQUAL(assign_node->data.binop.right->data.constant.value, 0);
    CU_ASSERT_NOT_EQUAL(root->next, NULL);
    ASTNode* next_program = root->next;
    if (next_program->type == NODE_TYPE_ASSIGN) {
        CU_ASSERT_EQUAL(next_program->data.assign.variable->key, 2);
        ASTNode* next_assign_node = next_program->data.assign.value;
        CU_ASSERT_EQUAL(next_assign_node->type, NODE_TYPE_BINARY_OP);
        CU_ASSERT_EQUAL(next_assign_node->data.binop.left->data.variable_access.variable->key, 2);
        CU_ASSERT_EQUAL(next_assign_node->data.binop.operation, ADDITION);
        CU_ASSERT_EQUAL(next_assign_node->data.binop.right->data.constant.value, 0);
    } else {
        CU_ASSERT_EQUAL(next_program->type, NODE_TYPE_ASSIGN);
        printf("\nwrong node type: %d\n", next_program->type);
    }

    free_program(program);
}

void test_parse_tokens_loop() {
    char *str_program = "Loop x1 Do\nx2 = x2 + 1;\nEnd";
    Program* program = create_program_from_config(str_program, (int[]){}, 0);
    ASTNode* root = program->start_node;

    CU_ASSERT_EQUAL(root->type, NODE_TYPE_LOOP);
    CU_ASSERT_EQUAL(root->data.for_loop.count_var->data.variable_access.variable->key, 1);
    ASTNode* body = root->data.for_loop.body;
    if (body->type == NODE_TYPE_ASSIGN) {
        CU_ASSERT_EQUAL(body->data.assign.variable->key, 2);
        ASTNode* assign = body->data.assign.value;
        if (assign->type == NODE_TYPE_BINARY_OP) {
            CU_ASSERT_EQUAL(assign->data.binop.left->data.variable_access.variable->key, 2);
            CU_ASSERT_EQUAL(assign->data.binop.operation, ADDITION);
            CU_ASSERT_EQUAL(assign->data.binop.right->data.constant.value, 1);
        } else{
            fprintf(stderr, "\ninvalid assign value, expected operation, actual: %d\n", assign->type);
            CU_FAIL();
        }

    } else {
        CU_ASSERT_EQUAL(root->data.for_loop.body->type, NODE_TYPE_ASSIGN);
        fprintf(stderr, "\nwrong body node type: %d\n", body->type);
    }

    free_program(program);
}

void test_parse_tokens_while_loop() {
    char *str_program = "While x1 > 0 Do\nx2 = x2 - 1;\nEnd";
    Program* program = create_program_from_config(str_program, (int[]){}, 0);
    ASTNode* root = program->start_node;

    CU_ASSERT_EQUAL(root->type, NODE_TYPE_WHILE);
    CU_ASSERT_EQUAL(root->data.while_loop.condition->data.variable_access.variable->key, 1);
    ASTNode* body = root->data.while_loop.body;
    if (body->type == NODE_TYPE_ASSIGN) {
        CU_ASSERT_EQUAL(body->data.assign.variable->key, 2);
        ASTNode* assign = body->data.assign.value;
        if (assign->type == NODE_TYPE_BINARY_OP) {
            CU_ASSERT_EQUAL(assign->data.binop.left->data.variable_access.variable->key, 2);
            CU_ASSERT_EQUAL(assign->data.binop.operation, SUBTRACTION);
            CU_ASSERT_EQUAL(assign->data.binop.right->data.constant.value, 1);
        } else{
            fprintf(stderr, "\ninvalid assign value, expected operation, actual: %d\n", assign->type);
            CU_FAIL();
        }

    } else {
        CU_ASSERT_EQUAL(root->data.while_loop.body->type, NODE_TYPE_ASSIGN);
        fprintf(stderr, "\nwrong body node type: %d\n", body->type);
    }

    free_program(program);
}

void test_parse_tokens_assign_multi_vars() {
    char *str_program = "x1 = x1 - x2;";
    Program *program = create_program_from_config(str_program, NULL, 0);
    ASTNode* root = program->start_node;

    CU_ASSERT_EQUAL(root->type, NODE_TYPE_ASSIGN);
    ASTNode* binop = root->data.assign.value;
    CU_ASSERT_EQUAL(binop->data.binop.left->type, NODE_TYPE_VARIABLE_ACCESS);
    CU_ASSERT_EQUAL(binop->data.binop.right->type, NODE_TYPE_VARIABLE_ACCESS);
    free_program(program);
}

void test_parse_tokens_const_then_var() {
    char *str_program = "x1 = 2 - x1;";
    Program* program = create_program_from_config(str_program, NULL, 0);
    ASTNode* root = program->start_node;

    CU_ASSERT_EQUAL(root->type, NODE_TYPE_ASSIGN);
    ASTNode* binop = root->data.assign.value;
    CU_ASSERT_EQUAL(binop->data.binop.left->type, NODE_TYPE_CONSTANT);
    CU_ASSERT_EQUAL(binop->data.binop.left->data.constant.value, 2);
    CU_ASSERT_EQUAL(binop->data.binop.operation, SUBTRACTION);
    CU_ASSERT_EQUAL(binop->data.binop.right->type, NODE_TYPE_VARIABLE_ACCESS);
    CU_ASSERT_EQUAL(binop->data.binop.right->data.variable_access.variable->key, 1);

    free_program(program);
}

void suite_parse_tokens(CU_pSuite suite) {
    CU_add_test(suite, "test_parse_basic_tokens", test_parse_basic_tokens);
    CU_add_test(suite, "test_parse_multi_tokens", test_parse_multi_tokens);
    CU_add_test(suite, "test_parse_tokens_loop", test_parse_tokens_loop);
    CU_add_test(suite, "test_parse_tokens_while_loop", test_parse_tokens_while_loop);
    CU_add_test(suite, "test_parse_tokens_assign_multi_vars", test_parse_tokens_assign_multi_vars);
    CU_add_test(suite, "test_parse_tokens_const_then_var", test_parse_tokens_const_then_var);
}

