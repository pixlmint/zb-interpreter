#include <CUnit/Basic.h>
#ifndef TEST_SUITES_H
#define TEST_SUITES_H


void suite_parser(CU_pSuite suite);
void suite_tree_builder(CU_pSuite suite);
void suite_parse_tokens(CU_pSuite suite);
void suite_interpreter(CU_pSuite suite);
void suite_examples(CU_pSuite suite);

#endif // TEST_SUITES_H
