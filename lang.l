%{
// lang lexer definition file
//
// Author: Daniel Stelle daniel.stelle@oit.edu

#include "tokens.h"

#define RETURN(a) return Return(a);
static int Return(int val);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here */
punctuation         [\.\,\;\[\]\{\}\(\)]
operators           [+\-*/%=!<]
identifiers         [a-zA-Z_][a-zA-z_0-9]*
integer_const       -?[0-9]+
float_const         -?[0-9]+[\.][0-9]+
whitespace          [' '\t\n]+
comment             "//".*$
junk                .

%%

    // token definitions go here.
"if"                        RETURN(IF);
"then"                      RETURN(THEN);
"else"                      RETURN(ELSE);
"for"                       RETURN(FOR);
"while"                     RETURN(WHILE);
"print"                     RETURN(PRINT);
"scan"                      RETURN(SCAN);
"int"                       RETURN(INT);
"float"                     RETURN(FLOAT);
"struct"                    RETURN(STRUCT);
"return"                    RETURN(RETURN_TOK);
"char"                      RETURN(CHAR);
"end"                       RETURN(END);
{punctuation}               RETURN(yytext[0]);
{operators}                 RETURN(yytext[0]);
{identifiers}               RETURN(IDENTIFIER);
{integer_const}             RETURN(INT_VAL);
{float_const}               RETURN(FLOAT_VAL);
{whitespace}                {};
{comment}                   {};
{junk}                      RETURN(JUNK_TOKEN);

%%

// This function allows us to do extra processing on each token
static int Return(int val)
{
//    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

