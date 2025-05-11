#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "csv_writer.h"

extern int yyparse();
extern FILE* yyin;
extern ASTNode* root;

int main(int argc, char** argv) {
    int print_ast_flag = 0;
    int generate_csv_flag = 0;
    char* input_file = NULL;
    char* output_dir = ".";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--print-ast") == 0) {
            print_ast_flag = 1;
        } else if (strcmp(argv[i], "--generate-csv") == 0) {
            generate_csv_flag = 1;
        } else if (strcmp(argv[i], "--output-dir") == 0 && i + 1 < argc) {
            output_dir = argv[++i];
        } else {
            input_file = argv[i];
        }
    }

    if (input_file) {
        yyin = fopen(input_file, "r");
        if (!yyin) {
            perror("Error opening input file");
            return EXIT_FAILURE;
        }
    } else {
        yyin = stdin;
    }

    if (yyparse() == 0) {
        if (!root) {
            fprintf(stderr, "Error: No AST generated\n");
            if (yyin != stdin) fclose(yyin);
            return EXIT_FAILURE;
        }
        
        if (print_ast_flag) {
            printf("Abstract Syntax Tree:\n");
            print_ast(root, 0);
        }
        
        if (generate_csv_flag) {
            printf("Generating CSV file in directory: %s\n", output_dir);
            generate_csv_from_ast(root, output_dir);
        }
        
        free_ast(root);
    } else {
        fprintf(stderr, "Parsing failed.\n");
        if (yyin != stdin) fclose(yyin);
        return EXIT_FAILURE;
    }

    if (yyin != stdin) fclose(yyin);
    
    return EXIT_SUCCESS;
}
