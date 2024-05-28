#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/zb_headers.h"

void run_calculation_test(char* file_path, int* input, int input_size, int expected_output) {
    char* code = read_file(file_path);
    Program* program = create_program_from_config(code, input, input_size);

    execute_ast(program->start_node, program->recursion_depth);

    int actual_value = program->x0->value;
    CU_ASSERT_EQUAL(expected_output, actual_value);
    if (expected_output != actual_value) {
        printf("expected: %d, actual: %d. source file: %s\n", expected_output, actual_value, file_path);
    }
    
    free_program(program);
    free(code);

}

void test_calculations() {
    printf("\n");
    run_calculation_test("examples/multiply.zb", (int[]) {2, 3}, 2, 6);
    run_calculation_test("examples/multiply.zb", (int[]) {3, 3}, 2, 9);
    run_calculation_test("examples/max.zb", (int[]) {8, 3}, 2, 8);
    run_calculation_test("examples/max.zb", (int[]) {16, 33}, 2, 33);
    run_calculation_test("examples/min.zb", (int[]) {16, 33}, 2, 16);
    run_calculation_test("examples/min.zb", (int[]) {8, 3}, 2, 3);
    run_calculation_test("examples/addition.zb", (int[]) {8, 3}, 2, 11);
    run_calculation_test("examples/addition.zb", (int[]) {88, 33}, 2, 121);
    run_calculation_test("examples/gcd.zb", (int[]) {8, 4}, 2, 4);
    run_calculation_test("examples/mod.zb", (int[]) {8, 3}, 2, 2);
    run_calculation_test("examples/mod.zb", (int[]) {8, 2}, 2, 0);
    run_calculation_test("examples/fibonacci.zb", (int[]) {1}, 1, 1);
    run_calculation_test("examples/fibonacci.zb", (int[]) {2}, 1, 2);
    run_calculation_test("examples/fibonacci.zb", (int[]) {3}, 1, 3);
    run_calculation_test("examples/fibonacci.zb", (int[]) {4}, 1, 5);
    run_calculation_test("examples/fibonacci.zb", (int[]) {5}, 1, 8);
    run_calculation_test("examples/fibonacci.zb", (int[]) {6}, 1, 13);
}

void suite_examples(CU_pSuite suite) {
    CU_add_test(suite, "calculations test", test_calculations);
}
