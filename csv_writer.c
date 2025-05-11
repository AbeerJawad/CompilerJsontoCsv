#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_writer.h"

static int global_id = 1;

void write_csv_header(FILE* f, const char* fields[], int count) {
    if (!f || !fields) return;
    
    fprintf(f, "id");
    for (int i = 0; i < count; i++) {
        fprintf(f, ",%s", fields[i] ? fields[i] : "");
    }
    fprintf(f, "\n");
}

void write_csv_row(FILE* f, int id, const char* values[], int count) {
    if (!f || !values) return;
    
    fprintf(f, "%d", id);
    for (int i = 0; i < count; i++) {
        fprintf(f, ",%s", values[i] ? values[i] : "");
    }
    fprintf(f, "\n");
}

void process_node(ASTNode* node, const char* headers[], const char* values[], int *count) {
    if (!node || *count >= 64) return;

    if (node->key) {
        headers[*count] = node->key;
        
        char valbuf[256] = {0};
        switch (node->type) {
            case AST_STRING:
                snprintf(valbuf, sizeof(valbuf), "\"%s\"", node->str_val ? node->str_val : "");
                break;
            case AST_NUMBER:
                snprintf(valbuf, sizeof(valbuf), "%g", node->num_val); // %g removes trailing zeros
                break;
            case AST_BOOL:
                snprintf(valbuf, sizeof(valbuf), "%s", node->bool_val ? "true" : "false");
                break;
            case AST_NULL:
                strcpy(valbuf, "null");
                break;
            case AST_OBJECT:
            case AST_ARRAY:
                strcpy(valbuf, "[complex]");
                break;
        }
        
        values[*count] = strdup(valbuf);
        (*count)++;
    }

    // Process children for nested structures
    if (node->type == AST_OBJECT || node->type == AST_ARRAY) {
        for (ASTNode* child = node->children; child; child = child->next) {
            process_node(child, headers, values, count);
        }
    }
}

void generate_csv_from_ast(ASTNode* root, const char* out_dir) {
    if (!root) {
        fprintf(stderr, "Error: NULL AST root\n");
        return;
    }

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/output.csv", out_dir ? out_dir : ".");
    
    FILE* f = fopen(filepath, "w");
    if (!f) {
        perror("Error creating CSV file");
        return;
    }

    const char* headers[64] = {0};
    const char* values[64] = {0};
    int count = 0;

    // Process top-level nodes
    for (ASTNode* child = root->children; child && count < 64; child = child->next) {
        process_node(child, headers, values, &count);
    }

    write_csv_header(f, headers, count);
    write_csv_row(f, global_id++, values, count);

    // Free allocated strings
    for (int i = 0; i < count; i++) {
        if (values[i]) free((char*)values[i]);
    }

    fclose(f);
}
