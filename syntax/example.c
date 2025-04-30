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
#line 3 "syntax/example.y"

#include <iostream>
#include "example.h"
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
#define YYNOCODE 68
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE const char*
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL
#define ParseARG_PDECL
#define ParseARG_PARAM
#define ParseARG_FETCH
#define ParseARG_STORE
#define ParseCTX_SDECL
#define ParseCTX_PDECL
#define ParseCTX_PARAM
#define ParseCTX_FETCH
#define ParseCTX_STORE
#define YYNSTATE             67
#define YYNRULE              62
#define YYNTOKEN             34
#define YY_MAX_SHIFT         66
#define YY_MIN_SHIFTREDUCE   115
#define YY_MAX_SHIFTREDUCE   176
#define YY_ERROR_ACTION      177
#define YY_ACCEPT_ACTION     178
#define YY_NO_ACTION         179
#define YY_MIN_REDUCE        180
#define YY_MAX_REDUCE        241
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
#define YY_ACTTAB_COUNT (196)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    30,  225,  225,  211,  189,   15,   40,   66,   20,   23,
 /*    10 */    14,   19,   16,   30,   19,   19,   51,   64,   15,  194,
 /*    20 */    17,   20,   23,   14,   19,  178,  162,   19,   19,   36,
 /*    30 */    49,  231,  215,   12,   15,  192,   17,   20,   43,   14,
 /*    40 */    19,  195,  162,   19,   19,   36,   42,   58,  168,  169,
 /*    50 */    15,   44,    7,   20,   10,   14,   19,  172,  173,   19,
 /*    60 */    19,    4,   31,   22,  168,  169,   53,    8,  172,  173,
 /*    70 */   186,   15,   47,   17,   20,    1,   14,   19,   27,   55,
 /*    80 */    19,   19,  170,  171,   15,  221,   13,   20,  210,   14,
 /*    90 */    19,  206,   59,   19,   19,  218,   11,   15,  184,   45,
 /*   100 */    20,  209,   14,   19,   60,  214,   19,   19,   34,   15,
 /*   110 */   181,   45,   20,   33,   14,   19,  122,  123,   19,   19,
 /*   120 */     3,  197,    9,    9,    9,    9,    9,    9,  162,   61,
 /*   130 */     9,    9,    9,    9,    9,    9,  219,  230,    7,   20,
 /*   140 */   205,   14,   19,   63,   24,   19,   19,   25,  126,   41,
 /*   150 */   168,  169,   28,   21,  172,  173,   26,  216,   48,   46,
 /*   160 */    20,  143,   14,   19,   18,   39,   19,   19,   29,  222,
 /*   170 */    50,   14,   19,    2,    5,   19,   19,  174,  175,  176,
 /*   180 */    32,   35,   56,   54,   52,    6,  142,   57,  159,  139,
 /*   190 */   180,   38,   62,  131,   65,   37,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    51,   65,   66,   54,   36,   56,    2,    1,   59,   41,
 /*    10 */    61,   62,    8,   51,   65,   66,   54,   36,   56,   42,
 /*    20 */    43,   59,   41,   61,   62,   34,    2,   65,   66,   51,
 /*    30 */    11,   53,   57,   58,   56,   42,   43,   59,    2,   61,
 /*    40 */    62,   40,    2,   65,   66,   51,   37,   53,   24,   25,
 /*    50 */    56,   40,   12,   59,   16,   61,   62,   28,   29,   65,
 /*    60 */    66,   12,   22,    6,   24,   25,   51,   18,   28,   29,
 /*    70 */    39,   56,   42,   43,   59,    8,   61,   62,   35,   51,
 /*    80 */    65,   66,   26,   27,   56,   63,   64,   59,   55,   61,
 /*    90 */    62,   37,   51,   65,   66,   60,   61,   56,   38,   39,
 /*   100 */    59,   55,   61,   62,   51,   37,   65,   66,   37,   56,
 /*   110 */    38,   39,   59,   23,   61,   62,    9,   10,   65,   66,
 /*   120 */     8,   44,   45,   46,   47,   48,   49,   50,    2,   44,
 /*   130 */    45,   46,   47,   48,   49,   50,   56,   67,   12,   59,
 /*   140 */    52,   61,   62,   37,   35,   65,   66,    7,    3,   15,
 /*   150 */    24,   25,    8,    3,   28,   29,    7,   56,    2,   13,
 /*   160 */    59,    3,   61,   62,   12,    2,   65,   66,   14,   59,
 /*   170 */    13,   61,   62,   12,   12,   65,   66,   30,   31,   32,
 /*   180 */    13,   13,   19,   20,   21,   12,    3,   13,   13,    3,
 /*   190 */     0,    3,    5,   17,    2,    4,   68,   68,   68,   68,
 /*   200 */    68,   68,   68,   68,   68,   68,   68,   68,   68,   68,
 /*   210 */    68,   68,   68,   68,   68,   68,   68,   68,   68,   68,
 /*   220 */    68,   68,   68,   68,   68,   68,   68,   68,   68,   68,
};
#define YY_SHIFT_COUNT    (66)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (192)
static const unsigned char yy_shift_ofst[] = {
 /*     0 */     6,   40,   40,  126,  126,  126,  126,  126,  126,  163,
 /*    10 */   163,  126,  126,  126,   24,  147,    4,    4,    4,   56,
 /*    20 */    29,   36,   36,   19,   19,  107,  107,   38,   36,   57,
 /*    30 */    67,   67,   38,   38,   90,   38,  112,   38,   57,   49,
 /*    40 */   140,  145,  134,  144,  150,  149,  154,  146,  152,  156,
 /*    50 */   158,  157,  161,  167,  162,  168,  173,  183,  174,  175,
 /*    60 */   186,  176,  190,  187,  191,  188,  192,
};
#define YY_REDUCE_COUNT (38)
#define YY_REDUCE_MIN   (-64)
#define YY_REDUCE_MAX   (110)
static const signed char yy_reduce_ofst[] = {
 /*     0 */    -9,  -51,  -38,  -22,   -6,   15,   28,   41,   53,   77,
 /*    10 */    85,   80,  101,  110,  -64,  -25,  -23,   -7,   30,   22,
 /*    20 */    35,   60,   72,  -32,  -19,    1,   11,    9,   31,   43,
 /*    30 */    33,   46,   54,   68,   88,   71,   70,  106,  109,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   177,  177,  177,  229,  229,  177,  177,  177,  177,  198,
 /*    10 */   198,  177,  177,  177,  177,  217,  193,  193,  193,  223,
 /*    20 */   220,  183,  177,  190,  190,  177,  177,  177,  177,  182,
 /*    30 */   212,  212,  177,  177,  213,  177,  232,  177,  182,  177,
 /*    40 */   177,  177,  177,  185,  177,  177,  177,  177,  177,  177,
 /*    50 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*    60 */   177,  177,  177,  177,  177,  177,  177,
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
  /*    2 */ "TKN_ID",
  /*    3 */ "TKN_SEMI_COLON",
  /*    4 */ "TKN_MAIN",
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
  /*   34 */ "program",
  /*   35 */ "vars",
  /*   36 */ "functions",
  /*   37 */ "body",
  /*   38 */ "vars_prm",
  /*   39 */ "vars_prm_prm",
  /*   40 */ "type",
  /*   41 */ "function",
  /*   42 */ "params",
  /*   43 */ "param",
  /*   44 */ "statements",
  /*   45 */ "statement",
  /*   46 */ "assign",
  /*   47 */ "condition",
  /*   48 */ "cycle",
  /*   49 */ "f_call",
  /*   50 */ "print",
  /*   51 */ "expression",
  /*   52 */ "cond",
  /*   53 */ "call",
  /*   54 */ "print_prm",
  /*   55 */ "print_prm_prm",
  /*   56 */ "exp",
  /*   57 */ "expression_prm",
  /*   58 */ "comp",
  /*   59 */ "termino",
  /*   60 */ "exp_prm",
  /*   61 */ "sign",
  /*   62 */ "factor",
  /*   63 */ "termino_prm",
  /*   64 */ "oper",
  /*   65 */ "factor_prm",
  /*   66 */ "cte",
  /*   67 */ "call_prm",
};
#endif /* defined(YYCOVERAGE) || !defined(NDEBUG) */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "program ::= TKN_PROGRAM TKN_ID TKN_SEMI_COLON vars functions TKN_MAIN body TKN_END",
 /*   1 */ "vars ::= TKN_VAR vars_prm",
 /*   2 */ "vars ::=",
 /*   3 */ "vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON",
 /*   4 */ "vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm",
 /*   5 */ "vars_prm_prm ::= TKN_ID",
 /*   6 */ "vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm",
 /*   7 */ "type ::= TKN_INT",
 /*   8 */ "type ::= TKN_FLOAT",
 /*   9 */ "functions ::= function functions",
 /*  10 */ "functions ::=",
 /*  11 */ "function ::= TKN_VOID TKN_ID TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET TKN_SEMI_COLON",
 /*  12 */ "params ::= param params",
 /*  13 */ "params ::=",
 /*  14 */ "params ::= TKN_COMMA params",
 /*  15 */ "param ::= TKN_ID TKN_COLON type",
 /*  16 */ "body ::= TKN_LBRACE statements TKN_RBRACE",
 /*  17 */ "statements ::= statement statements",
 /*  18 */ "statements ::=",
 /*  19 */ "statement ::= assign",
 /*  20 */ "statement ::= condition",
 /*  21 */ "statement ::= cycle",
 /*  22 */ "statement ::= f_call",
 /*  23 */ "statement ::= print",
 /*  24 */ "assign ::= TKN_ID TKN_ASSIGN expression TKN_SEMI_COLON",
 /*  25 */ "condition ::= TKN_IF TKN_LPAREN expression TKN_RPAREN body cond",
 /*  26 */ "cycle ::= TKN_WHILE TKN_LPAREN expression TKN_RPAREN body",
 /*  27 */ "f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON",
 /*  28 */ "print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON",
 /*  29 */ "print_prm ::= TKN_STRING_CONST print_prm_prm",
 /*  30 */ "print_prm ::= expression print_prm_prm",
 /*  31 */ "print_prm_prm ::= TKN_COMMA print_prm",
 /*  32 */ "print_prm_prm ::=",
 /*  33 */ "cond ::=",
 /*  34 */ "cond ::= TKN_ELSE body",
 /*  35 */ "expression ::= exp expression_prm",
 /*  36 */ "expression_prm ::= comp exp",
 /*  37 */ "expression_prm ::=",
 /*  38 */ "exp ::= termino exp_prm",
 /*  39 */ "exp_prm ::= sign exp",
 /*  40 */ "exp_prm ::=",
 /*  41 */ "termino ::= factor termino_prm",
 /*  42 */ "termino_prm ::= oper termino",
 /*  43 */ "termino_prm ::=",
 /*  44 */ "factor ::= TKN_LPAREN expression TKN_RPAREN",
 /*  45 */ "factor ::= sign factor_prm",
 /*  46 */ "factor ::= factor_prm",
 /*  47 */ "factor_prm ::= TKN_ID",
 /*  48 */ "factor_prm ::= cte",
 /*  49 */ "call ::=",
 /*  50 */ "call ::= expression call_prm",
 /*  51 */ "call_prm ::= TKN_COMMA call",
 /*  52 */ "call_prm ::=",
 /*  53 */ "cte ::= TKN_INT_CONST",
 /*  54 */ "cte ::= TKN_FLOAT_CONST",
 /*  55 */ "oper ::= TKN_MULT",
 /*  56 */ "oper ::= TKN_DIV",
 /*  57 */ "sign ::= TKN_PLUS",
 /*  58 */ "sign ::= TKN_MINUS",
 /*  59 */ "comp ::= TKN_LT",
 /*  60 */ "comp ::= TKN_GT",
 /*  61 */ "comp ::= TKN_NE",
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
  {   34,   -8 }, /* (0) program ::= TKN_PROGRAM TKN_ID TKN_SEMI_COLON vars functions TKN_MAIN body TKN_END */
  {   35,   -2 }, /* (1) vars ::= TKN_VAR vars_prm */
  {   35,    0 }, /* (2) vars ::= */
  {   38,   -4 }, /* (3) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON */
  {   38,   -5 }, /* (4) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm */
  {   39,   -1 }, /* (5) vars_prm_prm ::= TKN_ID */
  {   39,   -3 }, /* (6) vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm */
  {   40,   -1 }, /* (7) type ::= TKN_INT */
  {   40,   -1 }, /* (8) type ::= TKN_FLOAT */
  {   36,   -2 }, /* (9) functions ::= function functions */
  {   36,    0 }, /* (10) functions ::= */
  {   41,  -10 }, /* (11) function ::= TKN_VOID TKN_ID TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET TKN_SEMI_COLON */
  {   42,   -2 }, /* (12) params ::= param params */
  {   42,    0 }, /* (13) params ::= */
  {   42,   -2 }, /* (14) params ::= TKN_COMMA params */
  {   43,   -3 }, /* (15) param ::= TKN_ID TKN_COLON type */
  {   37,   -3 }, /* (16) body ::= TKN_LBRACE statements TKN_RBRACE */
  {   44,   -2 }, /* (17) statements ::= statement statements */
  {   44,    0 }, /* (18) statements ::= */
  {   45,   -1 }, /* (19) statement ::= assign */
  {   45,   -1 }, /* (20) statement ::= condition */
  {   45,   -1 }, /* (21) statement ::= cycle */
  {   45,   -1 }, /* (22) statement ::= f_call */
  {   45,   -1 }, /* (23) statement ::= print */
  {   46,   -4 }, /* (24) assign ::= TKN_ID TKN_ASSIGN expression TKN_SEMI_COLON */
  {   47,   -6 }, /* (25) condition ::= TKN_IF TKN_LPAREN expression TKN_RPAREN body cond */
  {   48,   -5 }, /* (26) cycle ::= TKN_WHILE TKN_LPAREN expression TKN_RPAREN body */
  {   49,   -5 }, /* (27) f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON */
  {   50,   -5 }, /* (28) print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON */
  {   54,   -2 }, /* (29) print_prm ::= TKN_STRING_CONST print_prm_prm */
  {   54,   -2 }, /* (30) print_prm ::= expression print_prm_prm */
  {   55,   -2 }, /* (31) print_prm_prm ::= TKN_COMMA print_prm */
  {   55,    0 }, /* (32) print_prm_prm ::= */
  {   52,    0 }, /* (33) cond ::= */
  {   52,   -2 }, /* (34) cond ::= TKN_ELSE body */
  {   51,   -2 }, /* (35) expression ::= exp expression_prm */
  {   57,   -2 }, /* (36) expression_prm ::= comp exp */
  {   57,    0 }, /* (37) expression_prm ::= */
  {   56,   -2 }, /* (38) exp ::= termino exp_prm */
  {   60,   -2 }, /* (39) exp_prm ::= sign exp */
  {   60,    0 }, /* (40) exp_prm ::= */
  {   59,   -2 }, /* (41) termino ::= factor termino_prm */
  {   63,   -2 }, /* (42) termino_prm ::= oper termino */
  {   63,    0 }, /* (43) termino_prm ::= */
  {   62,   -3 }, /* (44) factor ::= TKN_LPAREN expression TKN_RPAREN */
  {   62,   -2 }, /* (45) factor ::= sign factor_prm */
  {   62,   -1 }, /* (46) factor ::= factor_prm */
  {   65,   -1 }, /* (47) factor_prm ::= TKN_ID */
  {   65,   -1 }, /* (48) factor_prm ::= cte */
  {   53,    0 }, /* (49) call ::= */
  {   53,   -2 }, /* (50) call ::= expression call_prm */
  {   67,   -2 }, /* (51) call_prm ::= TKN_COMMA call */
  {   67,    0 }, /* (52) call_prm ::= */
  {   66,   -1 }, /* (53) cte ::= TKN_INT_CONST */
  {   66,   -1 }, /* (54) cte ::= TKN_FLOAT_CONST */
  {   64,   -1 }, /* (55) oper ::= TKN_MULT */
  {   64,   -1 }, /* (56) oper ::= TKN_DIV */
  {   61,   -1 }, /* (57) sign ::= TKN_PLUS */
  {   61,   -1 }, /* (58) sign ::= TKN_MINUS */
  {   58,   -1 }, /* (59) comp ::= TKN_LT */
  {   58,   -1 }, /* (60) comp ::= TKN_GT */
  {   58,   -1 }, /* (61) comp ::= TKN_NE */
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
      default:
      /* (0) program ::= TKN_PROGRAM TKN_ID TKN_SEMI_COLON vars functions TKN_MAIN body TKN_END */ yytestcase(yyruleno==0);
      /* (1) vars ::= TKN_VAR vars_prm */ yytestcase(yyruleno==1);
      /* (2) vars ::= */ yytestcase(yyruleno==2);
      /* (3) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON */ yytestcase(yyruleno==3);
      /* (4) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm */ yytestcase(yyruleno==4);
      /* (5) vars_prm_prm ::= TKN_ID */ yytestcase(yyruleno==5);
      /* (6) vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm */ yytestcase(yyruleno==6);
      /* (7) type ::= TKN_INT */ yytestcase(yyruleno==7);
      /* (8) type ::= TKN_FLOAT */ yytestcase(yyruleno==8);
      /* (9) functions ::= function functions */ yytestcase(yyruleno==9);
      /* (10) functions ::= */ yytestcase(yyruleno==10);
      /* (11) function ::= TKN_VOID TKN_ID TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET TKN_SEMI_COLON */ yytestcase(yyruleno==11);
      /* (12) params ::= param params */ yytestcase(yyruleno==12);
      /* (13) params ::= */ yytestcase(yyruleno==13);
      /* (14) params ::= TKN_COMMA params */ yytestcase(yyruleno==14);
      /* (15) param ::= TKN_ID TKN_COLON type */ yytestcase(yyruleno==15);
      /* (16) body ::= TKN_LBRACE statements TKN_RBRACE */ yytestcase(yyruleno==16);
      /* (17) statements ::= statement statements */ yytestcase(yyruleno==17);
      /* (18) statements ::= */ yytestcase(yyruleno==18);
      /* (19) statement ::= assign (OPTIMIZED OUT) */ assert(yyruleno!=19);
      /* (20) statement ::= condition (OPTIMIZED OUT) */ assert(yyruleno!=20);
      /* (21) statement ::= cycle (OPTIMIZED OUT) */ assert(yyruleno!=21);
      /* (22) statement ::= f_call (OPTIMIZED OUT) */ assert(yyruleno!=22);
      /* (23) statement ::= print (OPTIMIZED OUT) */ assert(yyruleno!=23);
      /* (24) assign ::= TKN_ID TKN_ASSIGN expression TKN_SEMI_COLON */ yytestcase(yyruleno==24);
      /* (25) condition ::= TKN_IF TKN_LPAREN expression TKN_RPAREN body cond */ yytestcase(yyruleno==25);
      /* (26) cycle ::= TKN_WHILE TKN_LPAREN expression TKN_RPAREN body */ yytestcase(yyruleno==26);
      /* (27) f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON */ yytestcase(yyruleno==27);
      /* (28) print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON */ yytestcase(yyruleno==28);
      /* (29) print_prm ::= TKN_STRING_CONST print_prm_prm */ yytestcase(yyruleno==29);
      /* (30) print_prm ::= expression print_prm_prm */ yytestcase(yyruleno==30);
      /* (31) print_prm_prm ::= TKN_COMMA print_prm */ yytestcase(yyruleno==31);
      /* (32) print_prm_prm ::= */ yytestcase(yyruleno==32);
      /* (33) cond ::= */ yytestcase(yyruleno==33);
      /* (34) cond ::= TKN_ELSE body */ yytestcase(yyruleno==34);
      /* (35) expression ::= exp expression_prm */ yytestcase(yyruleno==35);
      /* (36) expression_prm ::= comp exp */ yytestcase(yyruleno==36);
      /* (37) expression_prm ::= */ yytestcase(yyruleno==37);
      /* (38) exp ::= termino exp_prm */ yytestcase(yyruleno==38);
      /* (39) exp_prm ::= sign exp */ yytestcase(yyruleno==39);
      /* (40) exp_prm ::= */ yytestcase(yyruleno==40);
      /* (41) termino ::= factor termino_prm */ yytestcase(yyruleno==41);
      /* (42) termino_prm ::= oper termino */ yytestcase(yyruleno==42);
      /* (43) termino_prm ::= */ yytestcase(yyruleno==43);
      /* (44) factor ::= TKN_LPAREN expression TKN_RPAREN */ yytestcase(yyruleno==44);
      /* (45) factor ::= sign factor_prm */ yytestcase(yyruleno==45);
      /* (46) factor ::= factor_prm (OPTIMIZED OUT) */ assert(yyruleno!=46);
      /* (47) factor_prm ::= TKN_ID */ yytestcase(yyruleno==47);
      /* (48) factor_prm ::= cte (OPTIMIZED OUT) */ assert(yyruleno!=48);
      /* (49) call ::= */ yytestcase(yyruleno==49);
      /* (50) call ::= expression call_prm */ yytestcase(yyruleno==50);
      /* (51) call_prm ::= TKN_COMMA call */ yytestcase(yyruleno==51);
      /* (52) call_prm ::= */ yytestcase(yyruleno==52);
      /* (53) cte ::= TKN_INT_CONST */ yytestcase(yyruleno==53);
      /* (54) cte ::= TKN_FLOAT_CONST */ yytestcase(yyruleno==54);
      /* (55) oper ::= TKN_MULT */ yytestcase(yyruleno==55);
      /* (56) oper ::= TKN_DIV */ yytestcase(yyruleno==56);
      /* (57) sign ::= TKN_PLUS */ yytestcase(yyruleno==57);
      /* (58) sign ::= TKN_MINUS */ yytestcase(yyruleno==58);
      /* (59) comp ::= TKN_LT */ yytestcase(yyruleno==59);
      /* (60) comp ::= TKN_GT */ yytestcase(yyruleno==60);
      /* (61) comp ::= TKN_NE */ yytestcase(yyruleno==61);
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
#line 9 "syntax/example.y"

  std::cout << "Syntax error! At: " << yyminor << std::endl;
#line 1203 "syntax/example.c"
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
