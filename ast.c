#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

//node with ast type
ASTNode* new_node(ASTType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed in new_node\n");
        return NULL;
    }
    memset(node, 0, sizeof(ASTNode)); //starting as 0
    node->type = type;
    return node;
}

//object of members
ASTNode* create_object_node(ASTNode* members) {
    ASTNode* node = new_node(AST_OBJECT);
    if (!node) return NULL;
    node->children = members;
    return node;
}

//array of elements
ASTNode* create_array_node(ASTNode* elements) {
    ASTNode* node = new_node(AST_ARRAY);
    if (!node) return NULL;
    node->children = elements;
    return node;
}

//string
ASTNode* create_string_node(char* val) {
    ASTNode* node = new_node(AST_STRING);
    if (!node) {
        free(val);
        return NULL;
    }
    
    if (val) {
        node->str_val = strdup(val);
        if (!node->str_val) {
            fprintf(stderr, "Memory allocation failed in create_string_node\n");
            free(node);
            free(val);
            return NULL;
        }
    }
    return node;
}

//number
ASTNode* create_number_node(double val) {
    ASTNode* node = new_node(AST_NUMBER);
    if (!node) return NULL;
    node->num_val = val;
    return node;
}

//bool
ASTNode* create_bool_node(int val) {
    ASTNode* node = new_node(AST_BOOL);
    if (!node) return NULL;
    node->bool_val = val;
    return node;
}

//if null
ASTNode* create_null_node() {
    return new_node(AST_NULL);
}

//key value pair
ASTNode* create_pair_node(char* key, ASTNode* val) {
    if (!val) {
        fprintf(stderr, "Error: Null value in create_pair_node\n");
        free(key);
        return NULL;
    }
    
    if (key) {
        val->key = strdup(key);
        if (!val->key) {
            fprintf(stderr, "Memory allocation failed in create_pair_node\n");
            free(key);
            return NULL;
        }
    }
    free(key);
    return val;
}

ASTNode* append_member(ASTNode* a, ASTNode* b) {
    if (!a) return b;
    if (!b) return a;
    
    ASTNode* temp = a;
    while (temp->next) temp = temp->next;
    temp->next = b;
    return a;
}

ASTNode* create_element_node(ASTNode* val) {
    return val;
}

ASTNode* append_element(ASTNode* a, ASTNode* b) {
    return append_member(a, b);
}

void print_indent(int indent) {
    for (int i = 0; i < indent; ++i) printf("  ");
}

void print_ast(ASTNode* node, int indent) {
    if (!node) return;
    print_indent(indent);
    
    switch (node->type) {
        case AST_OBJECT:
            printf("OBJECT\n");
            for (ASTNode* child = node->children; child; child = child->next)
                print_ast(child, indent + 1);
            break;
        case AST_ARRAY:
            printf("ARRAY\n");
            for (ASTNode* child = node->children; child; child = child->next)
                print_ast(child, indent + 1);
            break;
        case AST_STRING:
            printf("%s: STRING \"%s\"\n", 
                   (node->key ? node->key : "-"), 
                   (node->str_val ? node->str_val : "NULL"));
            break;
        case AST_NUMBER:
            printf("%s: NUMBER %f\n", 
                   (node->key ? node->key : "-"), 
                   node->num_val);
            break;
        case AST_BOOL:
            printf("%s: BOOL %s\n", 
                   (node->key ? node->key : "-"), 
                   (node->bool_val ? "true" : "false"));
            break;
        case AST_NULL:
            printf("%s: NULL\n", 
                   (node->key ? node->key : "-"));
            break;
    }
}

//free memory recursively:(((
void free_ast(ASTNode* node) {
    if (!node) return;
    
    free_ast(node->children);
    free_ast(node->next);
    
    if (node->key) free(node->key);
    if (node->type == AST_STRING && node->str_val) free(node->str_val);
    
    free(node);
}
