typedef enum {
    TOKEN_INT,         // For numeric constants
    TOKEN_VAR,         // For variables like x1, x2, etc.
    TOKEN_PLUS,        // For '+'
    TOKEN_MINUS,       // For '-'
    TOKEN_ASSIGN,      // For '='
    TOKEN_SEMICOLON,   // For ';'
    TOKEN_LPAREN,      // For '('
    TOKEN_RPAREN,      // For ')'
    TOKEN_LOOP,        // For "Loop"
    TOKEN_WHILE,       // For "While"
    TOKEN_DO,          // For "Do"
    TOKEN_END,         // For "End"
    TOKEN_GT,          // For '>'
    TOKEN_EOF,         // End of file/input token
    TOKEN_UNKNOWN,     // Unknown token
    TOKEN_ANY,         // fallback any token
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

typedef enum {
    NODE_TYPE_ASSIGN,
    NODE_TYPE_WHILE,
    NODE_TYPE_LOOP,
    NODE_TYPE_EXPR,
    NODE_TYPE_CONSTANT,
    NODE_TYPE_VARIABLE_ACCESS,
    NODE_TYPE_BINARY_OP,
    NODE_TYPE_UNINIZIALISED,
} NodeType;

typedef enum {
    ADDITION,
    SUBTRACTION,
} BinaryOperation;

typedef struct UserVar {
    int key;
    int value;
} UserVar;

typedef struct ASTNode {
    NodeType type;
    union {
        struct { struct ASTNode* left; struct ASTNode* right; BinaryOperation operation; } binop;  // For binary operations
        struct { UserVar* variable; struct ASTNode* value; } assign;      // For assignments
        struct { struct ASTNode* condition; struct ASTNode* body; } while_loop;  // For loops
        struct { struct ASTNode* count_var; struct ASTNode* body; } for_loop;  // For loops
        struct { UserVar* variable; } variable_access;
        struct { int value; } constant;
    } data;
    struct ASTNode* next; // For linking nodes in a sequence
} ASTNode;

typedef struct UserVarArray {
    UserVar* variables;
    int size;
    int capacity;
} UserVarArray;

UserVar* get_variable(int variable_key);
void free_variables(UserVarArray* vars);

typedef struct LinkedASTNodeList {
    ASTNode* head;
    ASTNode* tail;
} LinkedASTNodeList;

LinkedASTNodeList* create_node_array();
void add_node(LinkedASTNodeList* array, ASTNode* new_node);
ASTNode* create_node(NodeType type);

ASTNode* parse_expression(Token** tokens);

void initialize_user_variables();

