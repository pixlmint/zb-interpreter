#include <CUnit/Basic.h>
#include <stdio.h>
#include "tests.h"

void test_execute_assign() {
    clear_error_bucket();
    Program* program = create_program_from_config("x0 = x1 + 2", NULL, 0);

    execute_ast(program->start_node, program->recursion_depth);

    UserVar* result = get_variable(0, program);
    CU_ASSERT_EQUAL(result->value, 2);

    free_program(program);
    assert_no_error();
}

void test_execute_loop() {
    clear_error_bucket();
    char* str_program = "x1 = x1 + 3;\nLoop x1 Do\nx0 = x0 + 1;End";
    Program* program = create_program_from_config(str_program, (int[]){}, 0);

    execute_ast(program->start_node, program->recursion_depth);

    UserVar* x1 = get_variable(1, program);
    UserVar* x0 = get_variable(0, program);

    CU_ASSERT_EQUAL(x1->value, 3);
    CU_ASSERT_EQUAL(x0->value, 3);
    int iterations = program->start_node->next->data.for_loop.iterations;
    CU_ASSERT_EQUAL(iterations, 3);
    if (iterations != 3) {
        printf("\nunexpected number of iterations: expected=%d, actual=%d\n", 3, iterations);
    }

    free_program(program);
    assert_no_error();
}

void test_execute_while() {
    clear_error_bucket();
    char* str_program = "x1 = x1 + 3;\nWhile x1 > 0 Do\nx0 = x0 + 1;x1 = x1 - 1;End";
    Program* program = create_program_from_config(str_program, (int[]){}, 0);

    execute_ast(program->start_node, program->recursion_depth);

    UserVar* x1 = get_variable(1, program);
    UserVar* x0 = get_variable(0, program);

    CU_ASSERT_EQUAL(x1->value, 0);
    CU_ASSERT_EQUAL(x0->value, 3);
    int iterations = program->start_node->next->data.for_loop.iterations;
    CU_ASSERT_EQUAL(iterations, 3);
    if (iterations != 3) {
        printf("\nunexpected number of iterations: expected=%d, actual=%d\n", 3, iterations);
    }

    free_program(program);
    assert_no_error();
}

void test_negative_variable_values() {
    clear_error_bucket();
    char *str_program = "x0 = x0 - 2;";
    Program* program = create_program_from_config(str_program, NULL, 0);
    execute_ast(program->start_node, program->recursion_depth);

    UserVar* x0 = program->x0;

    CU_ASSERT_EQUAL(x0->value, 0);

    free_program(program);
    assert_no_error();
}


void suite_interpreter(CU_pSuite suite) {
    CU_add_test(suite, "test_parse_tokens_loop", test_execute_assign);
    CU_add_test(suite, "test_execute_loop", test_execute_loop);
    CU_add_test(suite, "test_execute_while", test_execute_while);
    CU_add_test(suite, "test_negative_variable_values", test_negative_variable_values);
}

