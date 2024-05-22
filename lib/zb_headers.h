// Enumerations
typedef enum {
    TOKEN_INT, TOKEN_VAR, TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASSIGN, TOKEN_SEMICOLON,
    TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LOOP, TOKEN_WHILE, TOKEN_DO, TOKEN_END,
    TOKEN_GT, TOKEN_EOF, TOKEN_UNKNOWN, TOKEN_ANY,
} TokenType;

typedef enum {
    NODE_TYPE_ASSIGN, NODE_TYPE_WHILE, NODE_TYPE_LOOP, NODE_TYPE_EXPR,
    NODE_TYPE_CONSTANT, NODE_TYPE_VARIABLE_ACCESS, NODE_TYPE_BINARY_OP,
    NODE_TYPE_UNINIZIALISED,
} NodeType;

typedef enum {
    ADDITION, SUBTRACTION,
} BinaryOperation;

// Structures
typedef struct {
    TokenType type;
    char* value;
} Token;

typedef struct UserVar {
    int key;
    int value;
} UserVar;

typedef struct ASTNode {
    NodeType type;
    union {
        struct { struct ASTNode* left; struct ASTNode* right; BinaryOperation operation; } binop;
        struct { UserVar* variable; struct ASTNode* value; } assign;
        struct { struct ASTNode* condition; struct ASTNode* body; int iterations;} while_loop;
        struct { struct ASTNode* count_var; struct ASTNode* body; int iterations;} for_loop;
        struct { UserVar* variable; } variable_access;
        struct { int value; } constant;
    } data;
    struct ASTNode* next;
} ASTNode;

typedef struct UserVarArray {
    UserVar* variables;
    int size;
    int capacity;
} UserVarArray;

typedef struct Program {
    UserVarArray* variables;
    ASTNode* start_node;
    UserVar* x0;
} Program;

typedef struct LinkedASTNodeList {
    ASTNode* head;
    ASTNode* tail;
} LinkedASTNodeList;

typedef struct {
    Token* tokens;
    int size;
    int capacity;
} TokenArray;

// Function declarations
UserVar* get_variable(int variable_key, Program* program);
void free_variables(UserVarArray* vars);
void free_program(Program* program);
Program* create_program_from_config(char* str_program, int* input, int input_size);
Program* init_program();
LinkedASTNodeList* create_node_array();
void add_node(LinkedASTNodeList* array, ASTNode* new_node);
ASTNode* create_node(NodeType type);
int execute_ast(ASTNode* node);
int str_to_int(char* value);
ASTNode* parse_expression(Token** tokens);
char* read_file(const char* filename);
TokenArray* create_token_array(int initial_capacity);
void add_token(TokenArray* array, Token token);
TokenArray* tokenize(char* input);
ASTNode* parse_tokens(TokenArray* tokens, Program* program);
int parse_statement(Token* current_token, Token* following_tokens, ASTNode** node);
int verify_next_tokens_equals(Token* tokens, TokenType* next_tokens, int current_position);
TokenArray* get_array_part(TokenArray* tokens, int start, int max_values);
int find_associated_end_tag(TokenArray* tokens, int start_position);
void free_ast_node(ASTNode* node);
void free_token_array(TokenArray* array);
