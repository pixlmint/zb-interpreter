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

#define MAX_VARS 100

typedef struct Expression {
    int type; // 0 for constant, 1 for variable
    int value;
} Expression;

typedef struct Assignment {
    int variableIndex;
    Expression *expr;
} Assignment;

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

typedef struct ASTNode {
    NodeType type;
    union {
        struct { struct ASTNode* left; struct ASTNode* right; BinaryOperation operation; } binop;  // For binary operations
        struct { int variable; struct ASTNode* value; } assign;      // For assignments
        struct { struct ASTNode* condition; struct ASTNode* body; } while_loop;  // For loops
        struct { struct ASTNode* count_var; struct ASTNode* body; } for_loop;  // For loops
        struct { int index; } variable_access;
        struct { int value; } constant;
    } data;
    struct ASTNode* next; // For linking nodes in a sequence
} ASTNode;

typedef struct LinkedASTNodeList {
    ASTNode* head;
    ASTNode* tail;
} LinkedASTNodeList;

LinkedASTNodeList* create_node_array();
void add_node(LinkedASTNodeList* array, ASTNode* new_node);
ASTNode* create_node(NodeType type);

ASTNode* parse_expression(Token** tokens);
int evaluate_expression(ASTNode* expr);

