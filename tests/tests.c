#include <CUnit/Basic.h>
#include "tests.h"

int main() {
    CU_initialize_registry();

    suite_parser(CU_add_suite("Tokenizer_Test_Suite", NULL, NULL));
    suite_tree_builder(CU_add_suite("Tree_Builder_Test_Suite", NULL, NULL));
    suite_parse_tokens(CU_add_suite("Parse_tokens_test_suite", NULL, NULL));
    suite_interpreter(CU_add_suite("Interpreter_suite", NULL, NULL));
    suite_examples(CU_add_suite("Examples Test Suite", NULL, NULL));

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
