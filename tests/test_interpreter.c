#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/tokenizer.h"

void test_execute_assign() {
    TokenArray* tokens = tokenize("x0 = x1 + 2");
    // ASTNode* root = parse_tokens(tokens);

    free_token_array(tokens);
    // free_ast_node(root);
}


void suite_interpreter(CU_pSuite suite) {

}
