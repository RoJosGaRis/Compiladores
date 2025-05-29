%type vars_prm_prm {StringList*}
%type params {StringList*}
%type cte_float {float}
%type cte_int {int}

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
  //printf("Program name: %s\n", ID);
}

program_end ::= TKN_END . {
  //printf("End of program\n");
  //ctx->programFunction = NULL;
  //ctx->currentFunction = NULL;
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

assign_start ::= TKN_ID(ID) TKN_ASSIGN(TA) . {
  handleOperator(ID, ctx);
  handleOperation(TA, ctx);
}
assign ::= assign_start expression TKN_SEMI_COLON . 

condition_start ::= TKN_RPAREN . {
  handleConditionStart(ctx);
}
condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON.

cycle_condition_start ::= TKN_WHILE TKN_LPAREN . {
  handleCycleConditionStart(ctx);
}
cycle_expression ::= expression . {
  handleCycleStart(ctx);
}
cycle_end ::= TKN_SEMI_COLON . {
  handleCycleEnd(ctx);
}
cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end.

f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON .

print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON .

string_to_print ::= TKN_STRING_CONST(TKN) . {
  handlePrintString(TKN, ctx);
}
expression_to_print ::= expression(E) .
{
  handlePrintExpression(E, ctx);
}
print_prm ::= string_to_print print_prm_prm .
print_prm ::= expression_to_print print_prm_prm .

print_prm_prm ::= TKN_COMMA print_prm .
print_prm_prm ::= .

cond ::= . {
  handleConditionEnd(ctx);
}

else_start ::= TKN_ELSE .
{
  handleConditionElseStart(ctx);
}
cond ::= else_start body . {
  handleConditionElseEnd(ctx);
}

expression ::= exp.
{
  quadsSolve(ctx);
}
expression ::= exp comp exp .  {
  quadsSolve(ctx);
}
exp ::= termino .
exp_begin ::= termino sign(S) .
{
  handleOperation(S, ctx);
}
exp ::= exp_begin exp .

termino ::= factor(F) .
{
  //handleOperator(F, ctx);
}
termino_begin ::= factor(F) oper(O) .
{
  //handleOperator(F, ctx);
  handleOperation(O, ctx);
}
termino ::= termino_begin termino . 

factor ::= TKN_LPAREN expression TKN_RPAREN . 

factor ::= sign factor_prm . 
factor ::= factor_prm .
factor_prm ::= TKN_ID(F) . 
{
  handleOperator(F, ctx);
}
factor_prm ::= cte_int .
factor_prm ::= cte_float .

call ::= .
call ::= expression call_prm .

call_prm ::= TKN_COMMA call .
call_prm ::= .

cte_int ::= TKN_INT_CONST(TK) . 
{
  addConstantInt(TK, ctx);
}
cte_float ::= TKN_FLOAT_CONST(TK) .
{
  addConstantFloat(TK, ctx);
}

oper ::= TKN_MULT .
oper ::= TKN_DIV .

sign ::= TKN_PLUS .
sign ::= TKN_MINUS .

comp ::= TKN_LT(LT) . {
  handleOperation(LT, ctx);
}
comp ::= TKN_GT(GT) . {
  handleOperation(GT, ctx);
}
comp ::= TKN_NE(NE) . {
  handleOperation(NE, ctx);
}

%nonassoc TKN_INT TKN_FLOAT TKN_PRINT TKN_IF TKN_ELSE TKN_WHILE TKN_DO TKN_INT_CONST TKN_FLOAT_CONST TKN_STRING_CONST
