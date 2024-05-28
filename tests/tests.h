#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>
#include "../lib/zb_headers.h"


void suite_parser(CU_pSuite suite);
void suite_tree_builder(CU_pSuite suite);
void suite_parse_tokens(CU_pSuite suite);
void suite_interpreter(CU_pSuite suite);
void suite_examples(CU_pSuite suite);
void suite_helpers(CU_pSuite suite);
void suite_variables(CU_pSuite suite);

int assert_no_error();
void assert_error_thrown(ErrorCode code);
void assert_errors_thrown(ErrorCode* codes);

typedef struct ErrorBucket {
    ErrorCode* errors;
    int error_count;
    int capacity;
} ErrorBucket;

ErrorBucket* create_error_bucket();
void add_error(ErrorBucket* bucket, ErrorCode error);
void clear_error_bucket();

