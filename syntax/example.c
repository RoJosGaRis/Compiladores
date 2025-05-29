/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 8 "syntax/example.y"

#include "example.h"
#include "../utils/parserContext.h"
#include <assert.h>
#line 33 "syntax/example.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_PARAM     Code to pass %extra_argument as a subroutine parameter
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    ParseCTX_*         As ParseARG_ except for %extra_context
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYNTOKEN           Number of terminal symbols
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
**    YY_MIN_REDUCE      Minimum value for reduce actions
**    YY_MAX_REDUCE      Maximum value for reduce actions
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 79
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE char*
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  StringList* yy52;
  int yy60;
  float yy85;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL ParserContext* ctx;
#define ParseARG_PDECL ,ParserContext* ctx
#define ParseARG_PARAM ,ctx
#define ParseARG_FETCH ParserContext* ctx=yypParser->ctx;
#define ParseARG_STORE yypParser->ctx=ctx;
#define ParseCTX_SDECL
#define ParseCTX_PDECL
#define ParseCTX_PARAM
#define ParseCTX_FETCH
#define ParseCTX_STORE
#define YYNSTATE             69
#define YYNRULE              72
#define YYNTOKEN             34
#define YY_MAX_SHIFT         68
#define YY_MIN_SHIFTREDUCE   127
#define YY_MAX_SHIFTREDUCE   198
#define YY_ERROR_ACTION      199
#define YY_ACCEPT_ACTION     200
#define YY_NO_ACTION         201
#define YY_MIN_REDUCE        202
#define YY_MAX_REDUCE        273
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X.
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (254)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    23,   23,  242,   11,   11,   11,   11,   11,   11,    7,
 /*    10 */    44,  152,   50,    3,   50,   48,   43,  200,  238,   23,
 /*    20 */    23,  218,  205,   17,  234,   60,   58,   56,  256,   33,
 /*    30 */    32,  153,  154,   20,   31,   24,    9,   14,   23,   13,
 /*    40 */   218,   23,  145,  152,  155,  156,  157,   55,   33,   32,
 /*    50 */    59,    8,   20,   45,   24,    9,   14,   23,   13,   36,
 /*    60 */    23,  142,   10,  153,  154,   23,   23,  197,  198,  161,
 /*    70 */   162,   67,   23,   23,  262,  262,   17,  195,  196,   49,
 /*    80 */   197,  198,    5,  224,   23,   23,  215,   46,  137,  226,
 /*    90 */    57,   23,   23,   39,  210,  136,   12,  240,   20,  268,
 /*   100 */    24,    9,   14,   23,   13,   20,   23,   24,    9,   14,
 /*   110 */    23,   13,   39,   23,   47,  262,  207,  259,   64,   24,
 /*   120 */     9,   14,   23,   13,   20,   23,   24,    9,   14,   23,
 /*   130 */    13,   27,   23,   16,  133,   23,   23,   52,  255,   53,
 /*   140 */     1,  254,   23,   23,  141,   34,  251,  221,  138,   18,
 /*   150 */   128,    4,   37,  267,   23,   23,   38,   66,   40,   19,
 /*   160 */   127,   23,   23,   61,   21,   68,   25,   26,   20,   28,
 /*   170 */    24,    9,   14,   23,   13,   20,   23,   24,    9,   14,
 /*   180 */    23,   13,   62,   23,   15,   22,   29,  223,   51,   24,
 /*   190 */     9,   14,   23,   13,   20,   23,   24,    9,   14,   23,
 /*   200 */    13,   30,   23,  178,   65,   11,   11,   11,   11,   11,
 /*   210 */    11,    7,   23,   23,  152,    3,   54,    2,  175,   35,
 /*   220 */   139,    6,    8,  174,  186,  177,   63,  233,   41,  166,
 /*   230 */    42,  201,  201,  201,  153,  154,  201,  201,  197,  198,
 /*   240 */   201,  201,  201,  201,  201,  201,  201,  260,  201,   14,
 /*   250 */    23,   13,  201,   23,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    36,   37,   49,   50,   51,   52,   53,   54,   55,   56,
 /*    10 */     4,    4,   34,   60,   34,    4,    1,   38,   41,   36,
 /*    20 */    37,   57,   44,   46,   44,   19,   20,   21,   64,   65,
 /*    30 */    66,   24,   25,   69,   11,   71,   72,   73,   74,   75,
 /*    40 */    57,   77,   23,    4,   30,   31,   32,   64,   65,   66,
 /*    50 */    59,   12,   69,   45,   71,   72,   73,   74,   75,   68,
 /*    60 */    77,   22,   70,   24,   25,   36,   37,   28,   29,    9,
 /*    70 */    10,   41,   36,   37,   36,   37,   46,   26,   27,   45,
 /*    80 */    28,   29,   12,   73,   36,   37,   57,    4,   18,   76,
 /*    90 */    61,   36,   37,   57,   35,    2,   16,   48,   69,   63,
 /*   100 */    71,   72,   73,   74,   75,   69,   77,   71,   72,   73,
 /*   110 */    74,   75,   57,   77,   42,   77,   34,   69,   63,   71,
 /*   120 */    72,   73,   74,   75,   69,   77,   71,   72,   73,   74,
 /*   130 */    75,   40,   77,    6,    4,   36,   37,   35,   67,   47,
 /*   140 */     8,   67,   36,   37,    2,   42,   62,   42,   13,   42,
 /*   150 */     5,    8,   58,   78,   36,   37,   57,   43,   42,   40,
 /*   160 */     4,   36,   37,   57,    7,   39,    8,   15,   69,    8,
 /*   170 */    71,   72,   73,   74,   75,   69,   77,   71,   72,   73,
 /*   180 */    74,   75,   57,   77,    2,    7,   14,   69,   13,   71,
 /*   190 */    72,   73,   74,   75,   69,   77,   71,   72,   73,   74,
 /*   200 */    75,   12,   77,    2,   49,   50,   51,   52,   53,   54,
 /*   210 */    55,   56,   36,   37,    4,   60,   13,   12,    2,   13,
 /*   220 */    12,   12,   12,    2,   13,    2,   13,    0,    3,   17,
 /*   230 */     2,   79,   79,   79,   24,   25,   79,   79,   28,   29,
 /*   240 */    79,   79,   79,   79,   79,   79,   79,   71,   79,   73,
 /*   250 */    74,   75,   79,   77,   79,   79,   79,   79,   79,   79,
 /*   260 */    79,   79,   79,   79,   79,
};
#define YY_SHIFT_COUNT    (68)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (228)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */    15,   39,   39,  210,  210,  210,  210,  210,  210,  210,
 /*    10 */   210,    6,    6,  210,    7,   11,   11,   23,   19,   23,
 /*    20 */    14,   60,   60,   51,   52,   83,   93,   80,   11,  127,
 /*    30 */    83,  130,  132,  132,  142,   80,   80,   80,  135,  143,
 /*    40 */   145,   80,  127,  156,   70,  158,  157,  152,  161,  182,
 /*    50 */   178,  172,  175,  189,  201,  203,  205,  206,  208,  216,
 /*    60 */   209,  221,  211,  223,  213,  212,  227,  225,  228,
};
#define YY_REDUCE_COUNT (43)
#define YY_REDUCE_MIN   (-47)
#define YY_REDUCE_MAX   (176)
static const short yy_reduce_ofst[] = {
 /*     0 */   -21,  -36,  -17,   29,   36,   55,   99,  106,  125,   48,
 /*    10 */   118,  -47,  155,  176,   38,  -22,  -20,  -23,   -9,   30,
 /*    20 */    -8,    8,   34,   13,   10,   59,   49,   72,   82,   91,
 /*    30 */   102,   92,   71,   74,   84,  103,  105,  107,   94,   75,
 /*    40 */   114,  116,  119,  126,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   199,  199,  199,  199,  266,  266,  199,  199,  199,  199,
 /*    10 */   199,  243,  243,  199,  199,  204,  199,  239,  219,  239,
 /*    20 */   222,  199,  199,  225,  258,  199,  199,  199,  199,  235,
 /*    30 */   199,  199,  257,  257,  199,  199,  199,  199,  199,  269,
 /*    40 */   199,  199,  235,  199,  199,  209,  199,  199,  206,  199,
 /*    50 */   199,  199,  199,  199,  199,  199,  199,  199,  199,  199,
 /*    60 */   199,  199,  199,  199,  199,  199,  199,  199,  199,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParseARG_SDECL                /* A place to hold %extra_argument */
  ParseCTX_SDECL                /* A place to hold %extra_context */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
  yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#if defined(YYCOVERAGE) || !defined(NDEBUG)
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  /*    0 */ "$",
  /*    1 */ "TKN_PROGRAM",
  /*    2 */ "TKN_SEMI_COLON",
  /*    3 */ "TKN_MAIN",
  /*    4 */ "TKN_ID",
  /*    5 */ "TKN_END",
  /*    6 */ "TKN_VAR",
  /*    7 */ "TKN_COLON",
  /*    8 */ "TKN_COMMA",
  /*    9 */ "TKN_INT",
  /*   10 */ "TKN_FLOAT",
  /*   11 */ "TKN_VOID",
  /*   12 */ "TKN_LPAREN",
  /*   13 */ "TKN_RPAREN",
  /*   14 */ "TKN_LBRACKET",
  /*   15 */ "TKN_RBRACKET",
  /*   16 */ "TKN_LBRACE",
  /*   17 */ "TKN_RBRACE",
  /*   18 */ "TKN_ASSIGN",
  /*   19 */ "TKN_IF",
  /*   20 */ "TKN_WHILE",
  /*   21 */ "TKN_PRINT",
  /*   22 */ "TKN_STRING_CONST",
  /*   23 */ "TKN_ELSE",
  /*   24 */ "TKN_INT_CONST",
  /*   25 */ "TKN_FLOAT_CONST",
  /*   26 */ "TKN_MULT",
  /*   27 */ "TKN_DIV",
  /*   28 */ "TKN_PLUS",
  /*   29 */ "TKN_MINUS",
  /*   30 */ "TKN_LT",
  /*   31 */ "TKN_GT",
  /*   32 */ "TKN_NE",
  /*   33 */ "TKN_DO",
  /*   34 */ "vars_prm_prm",
  /*   35 */ "params",
  /*   36 */ "cte_float",
  /*   37 */ "cte_int",
  /*   38 */ "program",
  /*   39 */ "program_id",
  /*   40 */ "vars",
  /*   41 */ "functions",
  /*   42 */ "body",
  /*   43 */ "program_end",
  /*   44 */ "vars_prm",
  /*   45 */ "type",
  /*   46 */ "function",
  /*   47 */ "function_id",
  /*   48 */ "function_end",
  /*   49 */ "statements",
  /*   50 */ "statement",
  /*   51 */ "assign",
  /*   52 */ "condition",
  /*   53 */ "cycle",
  /*   54 */ "f_call",
  /*   55 */ "print",
  /*   56 */ "assign_start",
  /*   57 */ "expression",
  /*   58 */ "condition_start",
  /*   59 */ "cond",
  /*   60 */ "cycle_condition_start",
  /*   61 */ "cycle_expression",
  /*   62 */ "cycle_end",
  /*   63 */ "call",
  /*   64 */ "print_prm",
  /*   65 */ "string_to_print",
  /*   66 */ "expression_to_print",
  /*   67 */ "print_prm_prm",
  /*   68 */ "else_start",
  /*   69 */ "exp",
  /*   70 */ "comp",
  /*   71 */ "termino",
  /*   72 */ "exp_begin",
  /*   73 */ "sign",
  /*   74 */ "factor",
  /*   75 */ "termino_begin",
  /*   76 */ "oper",
  /*   77 */ "factor_prm",
  /*   78 */ "call_prm",
};
#endif /* defined(YYCOVERAGE) || !defined(NDEBUG) */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "program_id ::= TKN_ID",
 /*   1 */ "program_end ::= TKN_END",
 /*   2 */ "vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON",
 /*   3 */ "vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm",
 /*   4 */ "vars_prm_prm ::= TKN_ID",
 /*   5 */ "vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm",
 /*   6 */ "function_id ::= TKN_ID",
 /*   7 */ "params ::= TKN_ID TKN_COLON type",
 /*   8 */ "params ::= TKN_ID TKN_COLON type TKN_COMMA params",
 /*   9 */ "function_end ::= TKN_SEMI_COLON",
 /*  10 */ "assign_start ::= TKN_ID TKN_ASSIGN",
 /*  11 */ "condition_start ::= TKN_RPAREN",
 /*  12 */ "cycle_condition_start ::= TKN_WHILE TKN_LPAREN",
 /*  13 */ "cycle_expression ::= expression",
 /*  14 */ "cycle_end ::= TKN_SEMI_COLON",
 /*  15 */ "string_to_print ::= TKN_STRING_CONST",
 /*  16 */ "expression_to_print ::= expression",
 /*  17 */ "cond ::=",
 /*  18 */ "else_start ::= TKN_ELSE",
 /*  19 */ "cond ::= else_start body",
 /*  20 */ "expression ::= exp",
 /*  21 */ "expression ::= exp comp exp",
 /*  22 */ "exp_begin ::= termino sign",
 /*  23 */ "termino ::= factor",
 /*  24 */ "termino_begin ::= factor oper",
 /*  25 */ "factor_prm ::= TKN_ID",
 /*  26 */ "cte_int ::= TKN_INT_CONST",
 /*  27 */ "cte_float ::= TKN_FLOAT_CONST",
 /*  28 */ "comp ::= TKN_LT",
 /*  29 */ "comp ::= TKN_GT",
 /*  30 */ "comp ::= TKN_NE",
 /*  31 */ "program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions TKN_MAIN body program_end",
 /*  32 */ "vars ::= TKN_VAR vars_prm",
 /*  33 */ "vars ::=",
 /*  34 */ "type ::= TKN_INT",
 /*  35 */ "type ::= TKN_FLOAT",
 /*  36 */ "functions ::= function functions",
 /*  37 */ "functions ::=",
 /*  38 */ "function ::= TKN_VOID function_id TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end",
 /*  39 */ "body ::= TKN_LBRACE statements TKN_RBRACE",
 /*  40 */ "statements ::= statement statements",
 /*  41 */ "statements ::=",
 /*  42 */ "statement ::= assign",
 /*  43 */ "statement ::= condition",
 /*  44 */ "statement ::= cycle",
 /*  45 */ "statement ::= f_call",
 /*  46 */ "statement ::= print",
 /*  47 */ "assign ::= assign_start expression TKN_SEMI_COLON",
 /*  48 */ "condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON",
 /*  49 */ "cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end",
 /*  50 */ "f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON",
 /*  51 */ "print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON",
 /*  52 */ "print_prm ::= string_to_print print_prm_prm",
 /*  53 */ "print_prm ::= expression_to_print print_prm_prm",
 /*  54 */ "print_prm_prm ::= TKN_COMMA print_prm",
 /*  55 */ "print_prm_prm ::=",
 /*  56 */ "exp ::= termino",
 /*  57 */ "exp ::= exp_begin exp",
 /*  58 */ "termino ::= termino_begin termino",
 /*  59 */ "factor ::= TKN_LPAREN expression TKN_RPAREN",
 /*  60 */ "factor ::= sign factor_prm",
 /*  61 */ "factor ::= factor_prm",
 /*  62 */ "factor_prm ::= cte_int",
 /*  63 */ "factor_prm ::= cte_float",
 /*  64 */ "call ::=",
 /*  65 */ "call ::= expression call_prm",
 /*  66 */ "call_prm ::= TKN_COMMA call",
 /*  67 */ "call_prm ::=",
 /*  68 */ "oper ::= TKN_MULT",
 /*  69 */ "oper ::= TKN_DIV",
 /*  70 */ "sign ::= TKN_PLUS",
 /*  71 */ "sign ::= TKN_MINUS",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void ParseInit(void *yypRawParser ParseCTX_PDECL){
  yyParser *yypParser = (yyParser*)yypRawParser;
  ParseCTX_STORE
#ifdef YYTRACKMAXSTACKDEPTH
  yypParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  yypParser->yytos = NULL;
  yypParser->yystack = NULL;
  yypParser->yystksz = 0;
  if( yyGrowStack(yypParser) ){
    yypParser->yystack = &yypParser->yystk0;
    yypParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  yypParser->yytos = yypParser->yystack;
  yypParser->yystack[0].stateno = 0;
  yypParser->yystack[0].major = 0;
#if YYSTACKDEPTH>0
  yypParser->yystackEnd = &yypParser->yystack[YYSTACKDEPTH-1];
#endif
}

#ifndef Parse_ENGINEALWAYSONSTACK
/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE) ParseCTX_PDECL){
  yyParser *yypParser;
  yypParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( yypParser ){
    ParseCTX_STORE
    ParseInit(yypParser ParseCTX_PARAM);
  }
  return (void*)yypParser;
}
#endif /* Parse_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH
  ParseCTX_FETCH
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void ParseFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef Parse_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  ParseFinalize(p);
  (*freeProc)(p);
}
#endif /* Parse_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/* This array of booleans keeps track of the parser statement
** coverage.  The element yycoverage[X][Y] is set when the parser
** is in state X and has a lookahead token Y.  In a well-tested
** systems, every element of this matrix should end up being set.
*/
#if defined(YYCOVERAGE)
static unsigned char yycoverage[YYNSTATE][YYNTOKEN];
#endif

/*
** Write into out a description of every state/lookahead combination that
**
**   (1)  has not been used by the parser, and
**   (2)  is not a syntax error.
**
** Return the number of missed state/lookahead combinations.
*/
#if defined(YYCOVERAGE)
int ParseCoverage(FILE *out){
  int stateno, iLookAhead, i;
  int nMissed = 0;
  for(stateno=0; stateno<YYNSTATE; stateno++){
    i = yy_shift_ofst[stateno];
    for(iLookAhead=0; iLookAhead<YYNTOKEN; iLookAhead++){
      if( yy_lookahead[i+iLookAhead]!=iLookAhead ) continue;
      if( yycoverage[stateno][iLookAhead]==0 ) nMissed++;
      if( out ){
        fprintf(out,"State %d lookahead %s %s\n", stateno,
                yyTokenName[iLookAhead],
                yycoverage[stateno][iLookAhead] ? "ok" : "missed");
      }
    }
  }
  return nMissed;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static YYACTIONTYPE yy_find_shift_action(
  YYCODETYPE iLookAhead,    /* The look-ahead token */
  YYACTIONTYPE stateno      /* Current state number */
){
  int i;

  if( stateno>YY_MAX_SHIFT ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
#if defined(YYCOVERAGE)
  yycoverage[stateno][iLookAhead] = 1;
#endif
  do{
    i = yy_shift_ofst[stateno];
    assert( i>=0 );
    assert( i+YYNTOKEN<=(int)sizeof(yy_lookahead)/sizeof(yy_lookahead[0]) );
    assert( iLookAhead!=YYNOCODE );
    assert( iLookAhead < YYNTOKEN );
    i += iLookAhead;
    if( yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  YYACTIONTYPE stateno,     /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParseARG_FETCH
   ParseCTX_FETCH
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE /* Suppress warning about unused %extra_argument var */
   ParseCTX_STORE
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState, const char *zTag){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%s%s '%s', go to state %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%s%s '%s', pending reduce %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState - YY_MIN_REDUCE);
    }
  }
}
#else
# define yyTraceShift(X,Y,Z)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  YYACTIONTYPE yyNewState,      /* The new state to shift in */
  YYCODETYPE yyMajor,           /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>yypParser->yystackEnd ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = yyNewState;
  yytos->major = yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState, "Shift");
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  {   39,   -1 }, /* (0) program_id ::= TKN_ID */
  {   43,   -1 }, /* (1) program_end ::= TKN_END */
  {   44,   -4 }, /* (2) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON */
  {   44,   -5 }, /* (3) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm */
  {   34,   -1 }, /* (4) vars_prm_prm ::= TKN_ID */
  {   34,   -3 }, /* (5) vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm */
  {   47,   -1 }, /* (6) function_id ::= TKN_ID */
  {   35,   -3 }, /* (7) params ::= TKN_ID TKN_COLON type */
  {   35,   -5 }, /* (8) params ::= TKN_ID TKN_COLON type TKN_COMMA params */
  {   48,   -1 }, /* (9) function_end ::= TKN_SEMI_COLON */
  {   56,   -2 }, /* (10) assign_start ::= TKN_ID TKN_ASSIGN */
  {   58,   -1 }, /* (11) condition_start ::= TKN_RPAREN */
  {   60,   -2 }, /* (12) cycle_condition_start ::= TKN_WHILE TKN_LPAREN */
  {   61,   -1 }, /* (13) cycle_expression ::= expression */
  {   62,   -1 }, /* (14) cycle_end ::= TKN_SEMI_COLON */
  {   65,   -1 }, /* (15) string_to_print ::= TKN_STRING_CONST */
  {   66,   -1 }, /* (16) expression_to_print ::= expression */
  {   59,    0 }, /* (17) cond ::= */
  {   68,   -1 }, /* (18) else_start ::= TKN_ELSE */
  {   59,   -2 }, /* (19) cond ::= else_start body */
  {   57,   -1 }, /* (20) expression ::= exp */
  {   57,   -3 }, /* (21) expression ::= exp comp exp */
  {   72,   -2 }, /* (22) exp_begin ::= termino sign */
  {   71,   -1 }, /* (23) termino ::= factor */
  {   75,   -2 }, /* (24) termino_begin ::= factor oper */
  {   77,   -1 }, /* (25) factor_prm ::= TKN_ID */
  {   37,   -1 }, /* (26) cte_int ::= TKN_INT_CONST */
  {   36,   -1 }, /* (27) cte_float ::= TKN_FLOAT_CONST */
  {   70,   -1 }, /* (28) comp ::= TKN_LT */
  {   70,   -1 }, /* (29) comp ::= TKN_GT */
  {   70,   -1 }, /* (30) comp ::= TKN_NE */
  {   38,   -8 }, /* (31) program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions TKN_MAIN body program_end */
  {   40,   -2 }, /* (32) vars ::= TKN_VAR vars_prm */
  {   40,    0 }, /* (33) vars ::= */
  {   45,   -1 }, /* (34) type ::= TKN_INT */
  {   45,   -1 }, /* (35) type ::= TKN_FLOAT */
  {   41,   -2 }, /* (36) functions ::= function functions */
  {   41,    0 }, /* (37) functions ::= */
  {   46,  -10 }, /* (38) function ::= TKN_VOID function_id TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end */
  {   42,   -3 }, /* (39) body ::= TKN_LBRACE statements TKN_RBRACE */
  {   49,   -2 }, /* (40) statements ::= statement statements */
  {   49,    0 }, /* (41) statements ::= */
  {   50,   -1 }, /* (42) statement ::= assign */
  {   50,   -1 }, /* (43) statement ::= condition */
  {   50,   -1 }, /* (44) statement ::= cycle */
  {   50,   -1 }, /* (45) statement ::= f_call */
  {   50,   -1 }, /* (46) statement ::= print */
  {   51,   -3 }, /* (47) assign ::= assign_start expression TKN_SEMI_COLON */
  {   52,   -7 }, /* (48) condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON */
  {   53,   -5 }, /* (49) cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end */
  {   54,   -5 }, /* (50) f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON */
  {   55,   -5 }, /* (51) print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON */
  {   64,   -2 }, /* (52) print_prm ::= string_to_print print_prm_prm */
  {   64,   -2 }, /* (53) print_prm ::= expression_to_print print_prm_prm */
  {   67,   -2 }, /* (54) print_prm_prm ::= TKN_COMMA print_prm */
  {   67,    0 }, /* (55) print_prm_prm ::= */
  {   69,   -1 }, /* (56) exp ::= termino */
  {   69,   -2 }, /* (57) exp ::= exp_begin exp */
  {   71,   -2 }, /* (58) termino ::= termino_begin termino */
  {   74,   -3 }, /* (59) factor ::= TKN_LPAREN expression TKN_RPAREN */
  {   74,   -2 }, /* (60) factor ::= sign factor_prm */
  {   74,   -1 }, /* (61) factor ::= factor_prm */
  {   77,   -1 }, /* (62) factor_prm ::= cte_int */
  {   77,   -1 }, /* (63) factor_prm ::= cte_float */
  {   63,    0 }, /* (64) call ::= */
  {   63,   -2 }, /* (65) call ::= expression call_prm */
  {   78,   -2 }, /* (66) call_prm ::= TKN_COMMA call */
  {   78,    0 }, /* (67) call_prm ::= */
  {   76,   -1 }, /* (68) oper ::= TKN_MULT */
  {   76,   -1 }, /* (69) oper ::= TKN_DIV */
  {   73,   -1 }, /* (70) sign ::= TKN_PLUS */
  {   73,   -1 }, /* (71) sign ::= TKN_MINUS */
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
**
** The yyLookahead and yyLookaheadToken parameters provide reduce actions
** access to the lookahead token (if any).  The yyLookahead will be YYNOCODE
** if the lookahead token has already been consumed.  As this procedure is
** only called from one place, optimizing compilers will in-line it, which
** means that the extra parameters have no performance impact.
*/
static YYACTIONTYPE yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno,       /* Number of the rule by which to reduce */
  int yyLookahead,             /* Lookahead token, or YYNOCODE if none */
  ParseTOKENTYPE yyLookaheadToken  /* Value of the lookahead token */
  ParseCTX_PDECL                   /* %extra_context */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH
  (void)yyLookahead;
  (void)yyLookaheadToken;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    if( yysize ){
      fprintf(yyTraceFILE, "%sReduce %d [%s], go to state %d.\n",
        yyTracePrompt,
        yyruleno, yyRuleName[yyruleno], yymsp[yysize].stateno);
    }else{
      fprintf(yyTraceFILE, "%sReduce %d [%s].\n",
        yyTracePrompt, yyruleno, yyRuleName[yyruleno]);
    }
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=yypParser->yystackEnd ){
      yyStackOverflow(yypParser);
      /* The call to yyStackOverflow() above pops the stack until it is
      ** empty, causing the main parser loop to exit.  So the return value
      ** is never used and does not matter. */
      return 0;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        /* The call to yyStackOverflow() above pops the stack until it is
        ** empty, causing the main parser loop to exit.  So the return value
        ** is never used and does not matter. */
        return 0;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* program_id ::= TKN_ID */
#line 23 "syntax/example.y"
{
  addFunction(&ctx->functionTable, yymsp[0].minor.yy0, "void");
  ctx->programFunction = findFunction(&ctx->functionTable, yymsp[0].minor.yy0);
  ctx->currentFunction = NULL;
  initContext(ctx);
  yymsp[0].minor.yy0 = strdup(yymsp[0].minor.yy0);
  //printf("Program name: %s\n", yymsp[0].minor.yy0);
}
#line 1134 "syntax/example.c"
        break;
      case 1: /* program_end ::= TKN_END */
#line 32 "syntax/example.y"
{
  //printf("End of program\n");
  //ctx->programFunction = NULL;
  //ctx->currentFunction = NULL;
}
#line 1143 "syntax/example.c"
        break;
      case 2: /* vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON */
#line 40 "syntax/example.y"
{
  handleVariableList(yymsp[-3].minor.yy52, ctx, yymsp[-1].minor.yy0);
}
#line 1150 "syntax/example.c"
        break;
      case 3: /* vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm */
#line 43 "syntax/example.y"
{
  handleVariableList(yymsp[-4].minor.yy52, ctx, yymsp[-2].minor.yy0);
}
#line 1157 "syntax/example.c"
        break;
      case 4: /* vars_prm_prm ::= TKN_ID */
#line 46 "syntax/example.y"
{
  yylhsminor.yy52 = malloc(sizeof(StringList));
  yylhsminor.yy52 ->id = strdup(yymsp[0].minor.yy0);
  yylhsminor.yy52 ->next = NULL;
}
#line 1166 "syntax/example.c"
  yymsp[0].minor.yy52 = yylhsminor.yy52;
        break;
      case 5: /* vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm */
#line 51 "syntax/example.y"
{
  yylhsminor.yy52 = malloc(sizeof(StringList));
  yylhsminor.yy52 ->id = strdup(yymsp[-2].minor.yy0);
  yylhsminor.yy52 ->next = yymsp[0].minor.yy52;
}
#line 1176 "syntax/example.c"
  yymsp[-2].minor.yy52 = yylhsminor.yy52;
        break;
      case 6: /* function_id ::= TKN_ID */
#line 65 "syntax/example.y"
{
  addFunction(&ctx->functionTable, yymsp[0].minor.yy0, "void");
  ctx->currentFunction = findFunction(&ctx->functionTable, yymsp[0].minor.yy0);
  //printf("Current Function: %s\n",yymsp[0].minor.yy0);
}
#line 1186 "syntax/example.c"
        break;
      case 7: /* params ::= TKN_ID TKN_COLON type */
#line 71 "syntax/example.y"
{
  yylhsminor.yy52 = malloc(sizeof(StringList*));
  yylhsminor.yy52->id = strdup(yymsp[-2].minor.yy0);
  yylhsminor.yy52->next = NULL;
  handleVariableList(yylhsminor.yy52, ctx, yymsp[0].minor.yy0);
}
#line 1196 "syntax/example.c"
  yymsp[-2].minor.yy52 = yylhsminor.yy52;
        break;
      case 8: /* params ::= TKN_ID TKN_COLON type TKN_COMMA params */
#line 77 "syntax/example.y"
{
  yylhsminor.yy52 = malloc(sizeof(StringList*));
  yylhsminor.yy52->id = strdup(yymsp[-4].minor.yy0);
  yylhsminor.yy52->next = NULL;
  handleVariableList(yylhsminor.yy52, ctx, yymsp[-2].minor.yy0);
}
#line 1207 "syntax/example.c"
  yymsp[-4].minor.yy52 = yylhsminor.yy52;
        break;
      case 9: /* function_end ::= TKN_SEMI_COLON */
#line 84 "syntax/example.y"
{
  ctx->currentFunction = NULL;
  //printf("Exiting function\n");
}
#line 1216 "syntax/example.c"
        break;
      case 10: /* assign_start ::= TKN_ID TKN_ASSIGN */
#line 100 "syntax/example.y"
{
  handleOperator(yymsp[-1].minor.yy0, ctx);
  handleOperation(yymsp[0].minor.yy0, ctx);
}
#line 1224 "syntax/example.c"
        break;
      case 11: /* condition_start ::= TKN_RPAREN */
#line 106 "syntax/example.y"
{
  handleConditionStart(ctx);
}
#line 1231 "syntax/example.c"
        break;
      case 12: /* cycle_condition_start ::= TKN_WHILE TKN_LPAREN */
#line 111 "syntax/example.y"
{
  handleCycleConditionStart(ctx);
}
#line 1238 "syntax/example.c"
        break;
      case 13: /* cycle_expression ::= expression */
#line 114 "syntax/example.y"
{
  handleCycleStart(ctx);
}
#line 1245 "syntax/example.c"
        break;
      case 14: /* cycle_end ::= TKN_SEMI_COLON */
#line 117 "syntax/example.y"
{
  handleCycleEnd(ctx);
}
#line 1252 "syntax/example.c"
        break;
      case 15: /* string_to_print ::= TKN_STRING_CONST */
#line 126 "syntax/example.y"
{
  handlePrintString(yymsp[0].minor.yy0, ctx);
}
#line 1259 "syntax/example.c"
        break;
      case 16: /* expression_to_print ::= expression */
#line 130 "syntax/example.y"
{
  handlePrintExpression(yymsp[0].minor.yy0, ctx);
}
#line 1266 "syntax/example.c"
        break;
      case 17: /* cond ::= */
#line 139 "syntax/example.y"
{
  handleConditionEnd(ctx);
}
#line 1273 "syntax/example.c"
        break;
      case 18: /* else_start ::= TKN_ELSE */
#line 144 "syntax/example.y"
{
  handleConditionElseStart(ctx);
}
#line 1280 "syntax/example.c"
        break;
      case 19: /* cond ::= else_start body */
#line 147 "syntax/example.y"
{
  handleConditionElseEnd(ctx);
}
#line 1287 "syntax/example.c"
        break;
      case 20: /* expression ::= exp */
      case 21: /* expression ::= exp comp exp */ yytestcase(yyruleno==21);
#line 152 "syntax/example.y"
{
  quadsSolve(ctx);
}
#line 1295 "syntax/example.c"
        break;
      case 22: /* exp_begin ::= termino sign */
      case 28: /* comp ::= TKN_LT */ yytestcase(yyruleno==28);
      case 29: /* comp ::= TKN_GT */ yytestcase(yyruleno==29);
      case 30: /* comp ::= TKN_NE */ yytestcase(yyruleno==30);
#line 160 "syntax/example.y"
{
  handleOperation(yymsp[0].minor.yy0, ctx);
}
#line 1305 "syntax/example.c"
        break;
      case 23: /* termino ::= factor */
#line 166 "syntax/example.y"
{
  //handleOperator(yymsp[0].minor.yy0, ctx);
}
#line 1312 "syntax/example.c"
        break;
      case 24: /* termino_begin ::= factor oper */
#line 170 "syntax/example.y"
{
  //handleOperator(yymsp[-1].minor.yy0, ctx);
  handleOperation(yymsp[0].minor.yy0, ctx);
}
#line 1320 "syntax/example.c"
        break;
      case 25: /* factor_prm ::= TKN_ID */
#line 181 "syntax/example.y"
{
  handleOperator(yymsp[0].minor.yy0, ctx);
}
#line 1327 "syntax/example.c"
        break;
      case 26: /* cte_int ::= TKN_INT_CONST */
#line 194 "syntax/example.y"
{
  addConstantInt(yymsp[0].minor.yy0, ctx);
}
#line 1334 "syntax/example.c"
        break;
      case 27: /* cte_float ::= TKN_FLOAT_CONST */
#line 198 "syntax/example.y"
{
  addConstantFloat(yymsp[0].minor.yy0, ctx);
}
#line 1341 "syntax/example.c"
        break;
      default:
      /* (31) program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions TKN_MAIN body program_end */ yytestcase(yyruleno==31);
      /* (32) vars ::= TKN_VAR vars_prm */ yytestcase(yyruleno==32);
      /* (33) vars ::= */ yytestcase(yyruleno==33);
      /* (34) type ::= TKN_INT */ yytestcase(yyruleno==34);
      /* (35) type ::= TKN_FLOAT */ yytestcase(yyruleno==35);
      /* (36) functions ::= function functions */ yytestcase(yyruleno==36);
      /* (37) functions ::= */ yytestcase(yyruleno==37);
      /* (38) function ::= TKN_VOID function_id TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end */ yytestcase(yyruleno==38);
      /* (39) body ::= TKN_LBRACE statements TKN_RBRACE */ yytestcase(yyruleno==39);
      /* (40) statements ::= statement statements */ yytestcase(yyruleno==40);
      /* (41) statements ::= */ yytestcase(yyruleno==41);
      /* (42) statement ::= assign (OPTIMIZED OUT) */ assert(yyruleno!=42);
      /* (43) statement ::= condition (OPTIMIZED OUT) */ assert(yyruleno!=43);
      /* (44) statement ::= cycle (OPTIMIZED OUT) */ assert(yyruleno!=44);
      /* (45) statement ::= f_call (OPTIMIZED OUT) */ assert(yyruleno!=45);
      /* (46) statement ::= print (OPTIMIZED OUT) */ assert(yyruleno!=46);
      /* (47) assign ::= assign_start expression TKN_SEMI_COLON */ yytestcase(yyruleno==47);
      /* (48) condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON */ yytestcase(yyruleno==48);
      /* (49) cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end */ yytestcase(yyruleno==49);
      /* (50) f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON */ yytestcase(yyruleno==50);
      /* (51) print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON */ yytestcase(yyruleno==51);
      /* (52) print_prm ::= string_to_print print_prm_prm */ yytestcase(yyruleno==52);
      /* (53) print_prm ::= expression_to_print print_prm_prm */ yytestcase(yyruleno==53);
      /* (54) print_prm_prm ::= TKN_COMMA print_prm */ yytestcase(yyruleno==54);
      /* (55) print_prm_prm ::= */ yytestcase(yyruleno==55);
      /* (56) exp ::= termino */ yytestcase(yyruleno==56);
      /* (57) exp ::= exp_begin exp */ yytestcase(yyruleno==57);
      /* (58) termino ::= termino_begin termino */ yytestcase(yyruleno==58);
      /* (59) factor ::= TKN_LPAREN expression TKN_RPAREN */ yytestcase(yyruleno==59);
      /* (60) factor ::= sign factor_prm */ yytestcase(yyruleno==60);
      /* (61) factor ::= factor_prm (OPTIMIZED OUT) */ assert(yyruleno!=61);
      /* (62) factor_prm ::= cte_int (OPTIMIZED OUT) */ assert(yyruleno!=62);
      /* (63) factor_prm ::= cte_float (OPTIMIZED OUT) */ assert(yyruleno!=63);
      /* (64) call ::= */ yytestcase(yyruleno==64);
      /* (65) call ::= expression call_prm */ yytestcase(yyruleno==65);
      /* (66) call_prm ::= TKN_COMMA call */ yytestcase(yyruleno==66);
      /* (67) call_prm ::= */ yytestcase(yyruleno==67);
      /* (68) oper ::= TKN_MULT */ yytestcase(yyruleno==68);
      /* (69) oper ::= TKN_DIV */ yytestcase(yyruleno==69);
      /* (70) sign ::= TKN_PLUS */ yytestcase(yyruleno==70);
      /* (71) sign ::= TKN_MINUS */ yytestcase(yyruleno==71);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[yysize].stateno,(YYCODETYPE)yygoto);

  /* There are no SHIFTREDUCE actions on nonterminals because the table
  ** generator has simplified them to pure REDUCE actions. */
  assert( !(yyact>YY_MAX_SHIFT && yyact<=YY_MAX_SHIFTREDUCE) );

  /* It is not possible for a REDUCE to be followed by an error */
  assert( yyact!=YY_ERROR_ACTION );

  yymsp += yysize+1;
  yypParser->yytos = yymsp;
  yymsp->stateno = (YYACTIONTYPE)yyact;
  yymsp->major = (YYCODETYPE)yygoto;
  yyTraceShift(yypParser, yyact, "... then shift");
  return yyact;
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH
  ParseCTX_FETCH
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParseARG_STORE /* Suppress warning about unused %extra_argument variable */
  ParseCTX_STORE
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH
  ParseCTX_FETCH
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 17 "syntax/example.y"

  printf("Syntax error! At: %s" , yyminor);
#line 1447 "syntax/example.c"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE /* Suppress warning about unused %extra_argument variable */
  ParseCTX_STORE
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH
  ParseCTX_FETCH
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE /* Suppress warning about unused %extra_argument variable */
  ParseCTX_STORE
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  YYACTIONTYPE yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser = (yyParser*)yyp;  /* The parser */
  ParseCTX_FETCH
  ParseARG_STORE

  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif

  yyact = yypParser->yytos->stateno;
#ifndef NDEBUG
  if( yyTraceFILE ){
    if( yyact < YY_MIN_REDUCE ){
      fprintf(yyTraceFILE,"%sInput '%s' in state %d\n",
              yyTracePrompt,yyTokenName[yymajor],yyact);
    }else{
      fprintf(yyTraceFILE,"%sInput '%s' with pending reduce %d\n",
              yyTracePrompt,yyTokenName[yymajor],yyact-YY_MIN_REDUCE);
    }
  }
#endif

  do{
    assert( yyact==yypParser->yytos->stateno );
    yyact = yy_find_shift_action(yymajor,yyact);
    if( yyact >= YY_MIN_REDUCE ){
      yyact = yy_reduce(yypParser,yyact-YY_MIN_REDUCE,yymajor,
                        yyminor ParseCTX_PARAM);
    }else if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      break;
    }else if( yyact==YY_ACCEPT_ACTION ){
      yypParser->yytos--;
      yy_accept(yypParser);
      return;
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
      if( yymajor==YYNOCODE ) break;
      yyact = yypParser->yytos->stateno;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      break;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      break;
#endif
    }
  }while( yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
