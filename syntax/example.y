%token_type {const char*}

%include {
#include <iostream>
#include "example.h"
#include <assert.h>
}

%syntax_error {
  std::cout << "Syntax error! At: " << yyminor << std::endl;
}


program ::= TKN_PROGRAM TKN_ID TKN_SEMI_COLON vars functions TKN_MAIN body TKN_END .

vars ::= TKN_VAR vars_prm .
vars ::= .
vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON .
vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm.
vars_prm_prm ::= TKN_ID .
vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm .

type ::= TKN_INT .
type ::= TKN_FLOAT .

functions ::= function functions .
functions ::= .

function ::= TKN_VOID TKN_ID TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET TKN_SEMI_COLON .

params ::= param params .
params ::= .
params ::= TKN_COMMA params .

param ::= TKN_ID TKN_COLON type .

body ::= TKN_LBRACE statements TKN_RBRACE .

statements ::= statement statements .
statements ::= .

statement ::= assign .
statement ::= condition .
statement ::= cycle .
statement ::= f_call .
statement ::= print .

assign ::= TKN_ID TKN_ASSIGN expression TKN_SEMI_COLON .
condition ::= TKN_IF TKN_LPAREN expression TKN_RPAREN body cond .
cycle ::= TKN_WHILE TKN_LPAREN expression TKN_RPAREN body .
f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON .
print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON .

print_prm ::= TKN_STRING_CONST print_prm_prm .
print_prm ::= expression print_prm_prm .

print_prm_prm ::= TKN_COMMA print_prm .
print_prm_prm ::= .

cond ::= .
cond ::= TKN_ELSE body .

expression ::= exp expression_prm .
expression_prm ::= comp exp .
expression_prm ::= .

exp ::= termino exp_prm .
exp_prm ::= sign exp .
exp_prm ::= .

termino ::= factor termino_prm .
termino_prm ::= oper termino .
termino_prm ::= .

factor ::= TKN_LPAREN expression TKN_RPAREN .

factor ::= sign factor_prm .
factor ::= factor_prm .
factor_prm ::= TKN_ID .
factor_prm ::= cte .

call ::= .
call ::= expression call_prm .

call_prm ::= TKN_COMMA call .
call_prm ::= .

cte ::= TKN_INT_CONST .
cte ::= TKN_FLOAT_CONST .

oper ::= TKN_MULT .
oper ::= TKN_DIV .

sign ::= TKN_PLUS .
sign ::= TKN_MINUS .

comp ::= TKN_LT .
comp ::= TKN_GT .
comp ::= TKN_NE .

%nonassoc TKN_INT TKN_FLOAT TKN_PRINT TKN_IF TKN_ELSE TKN_WHILE TKN_DO TKN_INT_CONST TKN_FLOAT_CONST TKN_STRING_CONST
