#include <CUnit/Basic.h>
#include "tests.h"
#include <CUnit/TestDB.h>
#include <stdlib.h>

ErrorBucket* ERROR_BUCKET;

void e_throw(ErrorCode error, char* message, int line) {
    add_error(ERROR_BUCKET, error);
}

int main() {
    ERROR_BUCKET = create_error_bucket();
    CU_initialize_registry();
    suite_parser(CU_add_suite("Tokenizer_Test_Suite", NULL, NULL));
    suite_variables(CU_add_suite("Variables Suite", NULL, NULL));
    suite_tree_builder(CU_add_suite("Tree_Builder_Test_Suite", NULL, NULL));
    suite_parse_tokens(CU_add_suite("Parse_tokens_test_suite", NULL, NULL));
    suite_interpreter(CU_add_suite("Interpreter_suite", NULL, NULL));
    suite_examples(CU_add_suite("Examples Test Suite", NULL, NULL));
    suite_helpers(CU_add_suite("Helpers Suite", NULL, NULL));

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    free(ERROR_BUCKET->errors);
    free(ERROR_BUCKET);
    return CU_get_error();
}

ErrorBucket* create_error_bucket() {
    ErrorBucket* bucket = malloc(sizeof(ErrorBucket));
    bucket->errors = malloc(sizeof(ErrorCode) * 10);
    bucket->capacity = 10;
    bucket->error_count = 0;

    return bucket;
}

void add_error(ErrorBucket* bucket, ErrorCode error) {
    if (bucket->error_count >= bucket->capacity) {
        bucket->capacity *= 2;
        bucket->errors = realloc(bucket->errors, bucket->capacity);
    }

    bucket->errors[bucket->error_count++] = error;
}

void clear_error_bucket() {
    free(ERROR_BUCKET->errors);
    ERROR_BUCKET->errors = malloc(sizeof(ErrorCode) * 10);
    ERROR_BUCKET->capacity = 10;
    ERROR_BUCKET->error_count = 0;
}

int assert_no_error() {
    CU_ASSERT_FALSE(ERROR_BUCKET->error_count);

    return ERROR_BUCKET->error_count;
}

void assert_error_thrown(ErrorCode error) {
    CU_ASSERT_NOT_EQUAL(ERROR_BUCKET->error_count, 0);
    for (int i = 0; i < ERROR_BUCKET->error_count; i++) {
        CU_ASSERT_EQUAL(ERROR_BUCKET->errors[i], error);
    }
}

