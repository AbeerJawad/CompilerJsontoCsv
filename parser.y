%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
extern int line_num, column_num;
extern FILE* yyin;
extern char* yytext;

ASTNode* root = NULL;

void yyerror(const char* message);
%}

//token types
%union {
    char* str_val;
    double num_val;
    int bool_val;
    struct ASTNode* ast_node;
}

//declaring
%token T_LBRACE T_RBRACE T_LBRACKET T_RBRACKET T_COLON T_COMMA
%token T_TRUE T_FALSE T_NULL
%token <str_val> T_STRING
%token <num_val> T_NUMBER

%type <ast_node> value object array pair members elements

//grammar
%%
start:
    value { 
        root = $1; 
    }
    ;

value:
    object      { $$ = $1; }
    | array     { $$ = $1; }
    | T_STRING  { $$ = create_string_node($1); if (!$$) YYERROR; }
    | T_NUMBER  { $$ = create_number_node($1); }
    | T_TRUE    { $$ = create_bool_node(1); }
    | T_FALSE   { $$ = create_bool_node(0); }
    | T_NULL    { $$ = create_null_node(); }
    ;

//rules
object:
    T_LBRACE members T_RBRACE { $$ = create_object_node($2); if (!$$) YYERROR; }
    | T_LBRACE T_RBRACE       { $$ = create_object_node(NULL); }
    ;

members:
    pair                     { $$ = $1; }
    | pair T_COMMA members   { $$ = append_member($1, $3); if (!$$) YYERROR; }
    ;

pair:
    T_STRING T_COLON value { 
        $$ = create_pair_node($1, $3); 
        if (!$$) {
            free($1);
            YYERROR;
        }
    }
    ;

array:
    T_LBRACKET elements T_RBRACKET { $$ = create_array_node($2); if (!$$) YYERROR; }
    | T_LBRACKET T_RBRACKET        { $$ = create_array_node(NULL); }
    ;

elements:
    value                       { $$ = create_element_node($1); }
    | value T_COMMA elements    { $$ = append_element($1, $3); if (!$$) YYERROR; }
    ;
%%

void yyerror(const char* message) {
    if (message) {
        fprintf(stderr, "Parser error at line %d, column %d: %s\n",
                line_num, column_num, message);
    } else {
        fprintf(stderr, "Parser error at line %d, column %d\n",
                line_num, column_num);
    }
    if (yytext) {
        fprintf(stderr, "Near token: %s\n", yytext);
    }
}
