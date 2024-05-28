#include "tests.h"
#include <stdlib.h>

Program* setup(int num_vars) {
    clear_error_bucket();
    Program* program = init_program();
    for (int i = 0; i < num_vars; i++) {
        if (i > program->variables->capacity) {
            program->variables->capacity *= 2;
            program->variables->variables = realloc(program->variables->variables, program->variables->capacity * sizeof(UserVar));
        }
        program->variables->variables[i] = (UserVar) {i, i + 1};
        program->variables->size = i + 1;
    }
    return program;
}

void teardown(Program* program) {
    free_program(program);
}

void test_get_variable_simple() {
    Program* program = setup(2);
    int initial_size = program->variables->size;
    UserVar* first_var = get_variable(0, program);
    CU_ASSERT_EQUAL(first_var->key, 0);
    CU_ASSERT_EQUAL(first_var->value, 1);
    CU_ASSERT_EQUAL(program->variables->size, initial_size);
    assert_no_error();
    teardown(program);
}

void get_get_variable_new() {
    Program* program = setup(2);
    CU_ASSERT_EQUAL(program->variables->size, 2);
    UserVar* new_var = get_variable(3, program);
    CU_ASSERT_EQUAL(program->variables->size, 3);
    CU_ASSERT_EQUAL(new_var->key, 3);
    assert_no_error();
    teardown(program);
}

void test_get_many_variables() {
    Program* program = setup(DEFAULT_VARS_CAPACITY);
    CU_ASSERT_EQUAL(program->variables->size, DEFAULT_VARS_CAPACITY);
    get_variable(DEFAULT_VARS_CAPACITY + 1, program);
    CU_ASSERT_EQUAL(program->variables->size, DEFAULT_VARS_CAPACITY + 1);
    CU_ASSERT_EQUAL(program->variables->capacity, DEFAULT_VARS_CAPACITY * 2);
    assert_no_error();
    teardown(program);
}

void suite_variables(CU_pSuite suite) {
    CU_add_test(suite, "test_get_variable_simple", test_get_variable_simple);
    CU_add_test(suite, "get_get_variable_new", get_get_variable_new);
    CU_add_test(suite, "test_get_many_variables", test_get_many_variables);
}
