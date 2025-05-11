%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>

// Track line and column position for error 
int line_num = 1;
int column_num = 1;

//update from counter
void update_position(char* text);
%}

%%
"{"            { update_position(yytext); return T_LBRACE; }
"}"            { update_position(yytext); return T_RBRACE; }
"["            { update_position(yytext); return T_LBRACKET; }
"]"            { update_position(yytext); return T_RBRACKET; }
":"            { update_position(yytext); return T_COLON; }
","            { update_position(yytext); return T_COMMA; }
"true"         { update_position(yytext); yylval.bool_val = 1; return T_TRUE; }
"false"        { update_position(yytext); yylval.bool_val = 0; return T_FALSE; }
"null"         { update_position(yytext); return T_NULL; }

\"([^\"\\]|\\.)*\"  {
    update_position(yytext);
    //strip quotes
    yylval.str_val = strdup(yytext + 1);
    if (!yylval.str_val) {
        fprintf(stderr, "Memory allocation failed at line %d\n", line_num);
        exit(EXIT_FAILURE);
    }
    size_t len = strlen(yylval.str_val);
    if (len > 0) {
        yylval.str_val[len - 1] = '\0';
    } else {
        free(yylval.str_val);
        yylval.str_val = strdup("");
        if (!yylval.str_val) {
            fprintf(stderr, "Memory allocation failed at line %d\n", line_num);
            exit(EXIT_FAILURE);
        }
    }
    return T_STRING;
}

-?[0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)? {
    update_position(yytext);
    yylval.num_val = atof(yytext);
    return T_NUMBER;
}

[ \t\r]+       { update_position(yytext);  } //skip whitespace
\n             { line_num++; column_num = 1; }
.              { 
    fprintf(stderr, "Lexical error at line %d, column %d: unexpected character '%s'\n", 
            line_num, column_num, yytext); 
    exit(EXIT_FAILURE); 
}

%%

void update_position(char* text) {
    if (!text) return;
    for (int i = 0; text[i]; i++) {
        if (text[i] == '\n') {
            line_num++;
            column_num = 1;
        } else {
            column_num++;
        }
    }
}

int yywrap(void) {
    return 1;
}
