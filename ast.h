#ifndef AST_H
#define AST_H

typedef enum { AST_OBJECT, AST_ARRAY, AST_STRING, AST_NUMBER, AST_BOOL, AST_NULL } ASTType;

typedef struct ASTNode {
    ASTType type;
    char* key;
    union {
        char* str_val;
        double num_val;
        int bool_val;
        struct ASTNode* children;
    };
    struct ASTNode* next;
} ASTNode;

ASTNode* create_object_node(ASTNode* members);
ASTNode* create_array_node(ASTNode* elements);
ASTNode* create_string_node(char* val);
ASTNode* create_number_node(double val);
ASTNode* create_bool_node(int val);
ASTNode* create_null_node();
ASTNode* create_pair_node(char* key, ASTNode* val);
ASTNode* append_member(ASTNode* a, ASTNode* b);
ASTNode* create_element_node(ASTNode* val);
ASTNode* append_element(ASTNode* a, ASTNode* b);
void print_ast(ASTNode* node, int indent);
void free_ast(ASTNode* node);

#endif
