%type vars_prm_prm {StringList*}
%type params {StringList*}

%token_type {char*}

%include {
#include "example.h"
#include "../utils/parserContext.h"
#include <assert.h>
}


%extra_argument {ParserContext* ctx}

%syntax_error {
  printf("Syntax error! At: %s" , yyminor);
}

program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions TKN_MAIN body program_end . 

program_id(ID) ::= TKN_ID(ID) . {
  addFunction(&ctx->functionTable, ID, "void");
  ctx->programFunction = findFunction(&ctx->functionTable, ID);
  ctx->currentFunction = NULL;
  initContext(ctx);
  ID = strdup(ID);
  printf("Program name: %s\n", ID);
}

program_end ::= TKN_END . {
  printf("End of program\n");
  ctx->programFunction = NULL;
  ctx->currentFunction = NULL;
}

vars ::= TKN_VAR vars_prm .
vars ::= .
vars_prm ::= vars_prm_prm(V) TKN_COLON type(T) TKN_SEMI_COLON . {
  handleVariableList(V, ctx, T);
}
vars_prm ::= vars_prm_prm(V) TKN_COLON type(T) TKN_SEMI_COLON vars_prm. {
  handleVariableList(V, ctx, T);
}
vars_prm_prm(V) ::= TKN_ID(T) . {
  V = malloc(sizeof(StringList));
  V ->id = strdup(T);
  V ->next = NULL;
}
vars_prm_prm(V) ::= TKN_ID(T) TKN_COMMA vars_prm_prm(VP) . {
  V = malloc(sizeof(StringList));
  V ->id = strdup(T);
  V ->next = VP;
}

type ::= TKN_INT .
type ::= TKN_FLOAT .

functions ::= function functions .
functions ::= .

function ::= TKN_VOID function_id TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end . 

function_id ::= TKN_ID(TI) . {
  addFunction(&ctx->functionTable, TI, "void");
  ctx->currentFunction = findFunction(&ctx->functionTable, TI);
  //printf("Current Function: %s\n",TI);
}

params(PARAMS) ::= TKN_ID(TI) TKN_COLON type(T). {
  PARAMS = malloc(sizeof(StringList*));
  PARAMS->id = strdup(TI);
  PARAMS->next = NULL;
  handleVariableList(PARAMS, ctx, T);
}
params(PARAMS) ::= TKN_ID(TI) TKN_COLON type(T) TKN_COMMA params .{
  PARAMS = malloc(sizeof(StringList*));
  PARAMS->id = strdup(TI);
  PARAMS->next = NULL;
  handleVariableList(PARAMS, ctx, T);
}

function_end ::= TKN_SEMI_COLON . {
  ctx->currentFunction = NULL;
  //printf("Exiting function\n");
}

body ::= TKN_LBRACE statements TKN_RBRACE .

statements ::= statement statements .
statements ::= .

statement ::= assign .
statement ::= condition .
statement ::= cycle .
statement ::= f_call .
statement ::= print .

assign ::= TKN_ID(ID) TKN_ASSIGN expression(E) TKN_SEMI_COLON . 
{
  printf("%s = %s\n", ID, E);
}
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

expression(E) ::= exp (E).
{
  //printf("=======Expression END: %s=======\n", E);
  quadsSolve(ctx);
}
expression ::= exp expression_prm . 
expression_prm ::= comp expression_end .
expression_end ::= exp (E).
{
  //printf("=======Expression END: %s=======\n", E);
}
exp ::= termino(T) .
{
  //printf("=======Termino: %s=======\n", T);
}
exp_begin ::= termino sign(S) .
{
  handleOperation(S, ctx);
}
exp ::= exp_begin exp .

termino ::= factor(F) .
{
  //printf("=======Factor: %s=======\n", F);
  handleOperator(F, ctx);
}
termino_begin ::= factor(F) oper(O) .
{
  //printf("=======Factor: %s Oper: %s=======\n", F, O);
  handleOperator(F, ctx);
  handleOperation(O, ctx);
}
termino ::= termino_begin termino . 

factor(F) ::= TKN_LPAREN expression(E) TKN_RPAREN . {
  F = malloc(sizeof(char) * (strlen(E) +1));
  sprintf(F, "%s", E);
}

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
