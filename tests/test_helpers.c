#include <CUnit/Basic.h>
#include "tests.h"
#include <stdlib.h>
#include <unistd.h>

void test_str_to_int() {
    clear_error_bucket();
    char* testVal[] = {"1", "55", "1000", "001"};
    int actualVal[] = {1, 55, 1000, 1};
    int result;
    for (int i = 0; i < 4; i++) {
        result = str_to_int(testVal[i]);
        CU_ASSERT(result == actualVal[i]);
    }
    assert_no_error();
}

void test_read_file() {
    clear_error_bucket();
    char* valid_file_name = "examples/addition.zb";
    if (access(valid_file_name, F_OK) != 0) {
        CU_FAIL();
        printf("\nError: The file %s does not exist in positive file read test\n", valid_file_name);
        return;
    }
    char *expected_content = "While x1 > 0 Do";
    char* file_content = read_file(valid_file_name);
    int is_match = strncmp(file_content, expected_content, strlen(expected_content)) == 0;
    CU_ASSERT_EQUAL(is_match, 1);
    free(file_content);
    assert_no_error();
}

void test_read_invalid_file() {
    clear_error_bucket();
    char *invalid_file_name = "inexistent.zb";
    if (access(invalid_file_name, F_OK) == 0) {
        CU_FAIL();
        printf("\nError: The file %s exists in the negative file read test\n", invalid_file_name);
        return;
    }

    CU_ASSERT_EQUAL(read_file(invalid_file_name), NULL);
    assert_error_thrown(E_READ_FILE_ERROR);
}

void suite_helpers(CU_pSuite suite) {
    CU_add_test(suite, "test_str_to_int", test_str_to_int);
    CU_add_test(suite, "test_read_file", test_read_file);
    CU_add_test(suite, "test_read_invalid_file", test_read_invalid_file);
}

