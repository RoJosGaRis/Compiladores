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
#line 17 "syntax/example.y"

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
#define YYNOCODE 83
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE char*
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  StringList* yy86;
  float yy93;
  int yy100;
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
#define YYNSTATE             70
#define YYNRULE              76
#define YYNTOKEN             34
#define YY_MAX_SHIFT         69
#define YY_MIN_SHIFTREDUCE   131
#define YY_MAX_SHIFTREDUCE   206
#define YY_ERROR_ACTION      207
#define YY_ACCEPT_ACTION     208
#define YY_NO_ACTION         209
#define YY_MIN_REDUCE        210
#define YY_MAX_REDUCE        285
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
#define YY_ACTTAB_COUNT (258)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    25,   25,  208,   45,  162,   44,  278,  278,  253,   46,
 /*    10 */    52,   63,   48,    7,   50,   25,   25,   52,   31,   64,
 /*    20 */    62,   58,   37,  232,  214,  163,  164,   25,   25,  205,
 /*    30 */   206,  248,  272,   33,   32,   25,   25,   22,  232,   26,
 /*    40 */    11,   14,   25,   13,  155,   25,   12,   57,   33,   32,
 /*    50 */    39,  278,   22,  150,   26,   11,   14,   25,   13,  144,
 /*    60 */    25,   25,   25,  240,   22,   47,   26,   11,   14,   25,
 /*    70 */    13,  162,   25,   51,  276,  238,   14,   25,   13,  143,
 /*    80 */    25,  165,  166,  167,  230,   25,   25,   54,   46,  252,
 /*    90 */   267,   34,  163,  164,   25,   25,  138,  139,   22,   10,
 /*   100 */    26,   11,   14,   25,   13,  250,   25,   42,  230,  216,
 /*   110 */    19,   49,   19,   17,   60,   34,   28,  226,    1,  203,
 /*   120 */   204,   61,   22,  140,   26,   11,   14,   25,   13,   55,
 /*   130 */    25,   22,    4,   26,   11,   14,   25,   13,  271,   25,
 /*   140 */    25,   25,  270,    3,  255,    9,    9,    9,    9,    9,
 /*   150 */     9,    8,   25,   25,  266,    5,  205,  206,  148,  264,
 /*   160 */    35,  145,  235,   65,   20,   38,  133,  132,   68,   40,
 /*   170 */    41,   25,   25,   21,  131,   66,   15,   22,   69,   26,
 /*   180 */    11,   14,   25,   13,   27,   25,    4,   25,   25,   22,
 /*   190 */   220,   26,   11,   14,   25,   13,   23,   25,   67,    9,
 /*   200 */     9,    9,    9,    9,    9,    8,   29,   16,  275,    5,
 /*   210 */    26,   11,   14,   25,   13,   24,   25,   30,  190,   53,
 /*   220 */    56,   18,    2,  149,  237,   59,   26,   11,   14,   25,
 /*   230 */    13,  162,   25,   36,  146,  184,    6,  183,  198,  247,
 /*   240 */     7,  209,   43,  175,  209,  209,  209,  209,  209,  209,
 /*   250 */   152,  209,  163,  164,  209,  209,  205,  206,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    37,   38,   41,    3,    3,    1,   37,   38,   35,   36,
 /*    10 */    34,   62,    3,   12,    3,   37,   38,   34,   11,   19,
 /*    20 */    20,   21,   73,   60,   48,   24,   25,   37,   38,   28,
 /*    30 */    29,   48,   69,   70,   71,   37,   38,   74,   60,   76,
 /*    40 */    77,   78,   79,   80,   23,   82,   75,   69,   70,   71,
 /*    50 */    60,   82,   74,   12,   76,   77,   78,   79,   80,   18,
 /*    60 */    82,   37,   38,   81,   74,   39,   76,   77,   78,   79,
 /*    70 */    80,    3,   82,   39,   76,   78,   78,   79,   80,    2,
 /*    80 */    82,   30,   31,   32,   60,   37,   38,   35,   36,   51,
 /*    90 */    66,   67,   24,   25,   37,   38,    9,   10,   74,   16,
 /*   100 */    76,   77,   78,   79,   80,   44,   82,   44,   60,   34,
 /*   110 */    49,   46,   49,    6,   66,   67,   43,   60,    8,   26,
 /*   120 */    27,   64,   74,    3,   76,   77,   78,   79,   80,   50,
 /*   130 */    82,   74,   40,   76,   77,   78,   79,   80,   72,   82,
 /*   140 */    37,   38,   72,    8,   52,   53,   54,   55,   56,   57,
 /*   150 */    58,   59,   37,   38,   68,   63,   28,   29,    2,   65,
 /*   160 */    46,   13,   46,   60,   46,   61,    5,    4,   47,   46,
 /*   170 */    45,   37,   38,   43,    3,   60,    8,   74,   42,   76,
 /*   180 */    77,   78,   79,   80,   15,   82,   40,   37,   38,   74,
 /*   190 */    13,   76,   77,   78,   79,   80,    7,   82,   52,   53,
 /*   200 */    54,   55,   56,   57,   58,   59,    8,    2,   74,   63,
 /*   210 */    76,   77,   78,   79,   80,    7,   82,   14,    2,   13,
 /*   220 */    13,   12,   12,    2,   74,   13,   76,   77,   78,   79,
 /*   230 */    80,    3,   82,   13,   12,    2,   12,    2,   13,    0,
 /*   240 */    12,   83,    2,   17,   83,   83,   83,   83,   83,   83,
 /*   250 */    22,   83,   24,   25,   83,   83,   28,   29,   83,   83,
 /*   260 */    83,   83,   83,   83,   83,   83,   83,   83,   83,   83,
 /*   270 */    83,   83,   83,   83,   83,   83,   83,   83,
};
#define YY_SHIFT_COUNT    (69)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (240)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */     4,  228,  228,    1,    1,    1,    1,    1,    1,    0,
 /*    10 */     0,    1,    1,    1,   68,    9,   11,   11,    9,    7,
 /*    20 */    21,    7,   51,   87,   87,   93,  128,   77,   83,   11,
 /*    30 */   107,  120,  110,  110,  135,  156,   83,   83,   83,  148,
 /*    40 */   161,   83,  163,  107,  171,   41,  168,  177,  189,  169,
 /*    50 */   198,  205,  208,  203,  206,  209,  216,  207,  210,  221,
 /*    60 */   212,  220,  222,  233,  224,  225,  235,  226,  239,  240,
};
#define YY_REDUCE_COUNT (44)
#define YY_REDUCE_MIN   (-51)
#define YY_REDUCE_MAX   (150)
static const short yy_reduce_ofst[] = {
 /*     0 */   -39,  -37,  -22,   24,   48,   57,  -10,  103,  115,   92,
 /*    10 */   146,  134,  150,   -2,  -31,  -27,  -24,  -17,   52,   61,
 /*    20 */   -51,   63,  -29,   26,   34,  -18,   -3,   38,   65,   75,
 /*    30 */    73,   79,   66,   70,   86,   94,  114,  116,  118,  104,
 /*    40 */   121,  123,  125,  130,  136,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   207,  207,  207,  265,  265,  207,  207,  207,  207,  256,
 /*    10 */   256,  207,  207,  207,  207,  207,  213,  207,  207,  251,
 /*    20 */   233,  251,  236,  207,  207,  239,  274,  207,  207,  207,
 /*    30 */   249,  207,  273,  273,  268,  207,  207,  207,  207,  207,
 /*    40 */   207,  207,  207,  249,  207,  207,  207,  221,  207,  207,
 /*    50 */   215,  207,  207,  207,  207,  207,  207,  207,  207,  207,
 /*    60 */   207,  207,  207,  207,  207,  207,  207,  207,  207,  207,
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
  /*    3 */ "TKN_ID",
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
  /*   34 */ "vars_prm_prm",
  /*   35 */ "params",
  /*   36 */ "param_def",
  /*   37 */ "cte_float",
  /*   38 */ "cte_int",
  /*   39 */ "type",
  /*   40 */ "f_call_start",
  /*   41 */ "program",
  /*   42 */ "program_id",
  /*   43 */ "vars",
  /*   44 */ "functions",
  /*   45 */ "program_main",
  /*   46 */ "body",
  /*   47 */ "program_end",
  /*   48 */ "vars_prm",
  /*   49 */ "function",
  /*   50 */ "function_start",
  /*   51 */ "function_end",
  /*   52 */ "statements",
  /*   53 */ "statement",
  /*   54 */ "assign",
  /*   55 */ "condition",
  /*   56 */ "cycle",
  /*   57 */ "f_call",
  /*   58 */ "print",
  /*   59 */ "assign_start",
  /*   60 */ "expression",
  /*   61 */ "condition_start",
  /*   62 */ "cond",
  /*   63 */ "cycle_condition_start",
  /*   64 */ "cycle_expression",
  /*   65 */ "cycle_end",
  /*   66 */ "call",
  /*   67 */ "call_expression",
  /*   68 */ "call_prm",
  /*   69 */ "print_prm",
  /*   70 */ "string_to_print",
  /*   71 */ "expression_to_print",
  /*   72 */ "print_prm_prm",
  /*   73 */ "else_start",
  /*   74 */ "exp",
  /*   75 */ "comp",
  /*   76 */ "termino",
  /*   77 */ "exp_begin",
  /*   78 */ "sign",
  /*   79 */ "factor",
  /*   80 */ "termino_begin",
  /*   81 */ "oper",
  /*   82 */ "factor_prm",
};
#endif /* defined(YYCOVERAGE) || !defined(NDEBUG) */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "program_id ::= TKN_ID",
 /*   1 */ "program_main ::= TKN_MAIN",
 /*   2 */ "program_end ::= TKN_END",
 /*   3 */ "vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON",
 /*   4 */ "vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm",
 /*   5 */ "vars_prm_prm ::= TKN_ID",
 /*   6 */ "vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm",
 /*   7 */ "type ::= TKN_INT",
 /*   8 */ "type ::= TKN_FLOAT",
 /*   9 */ "function_start ::= TKN_ID",
 /*  10 */ "params ::= TKN_ID TKN_COLON type",
 /*  11 */ "param_def ::= TKN_ID TKN_COLON type",
 /*  12 */ "function_end ::= TKN_SEMI_COLON",
 /*  13 */ "assign_start ::= TKN_ID TKN_ASSIGN",
 /*  14 */ "condition_start ::= TKN_RPAREN",
 /*  15 */ "cycle_condition_start ::= TKN_WHILE TKN_LPAREN",
 /*  16 */ "cycle_expression ::= expression",
 /*  17 */ "cycle_end ::= TKN_SEMI_COLON",
 /*  18 */ "f_call ::= f_call_start call TKN_RPAREN TKN_SEMI_COLON",
 /*  19 */ "f_call_start ::= TKN_ID TKN_LPAREN",
 /*  20 */ "call_expression ::= expression",
 /*  21 */ "string_to_print ::= TKN_STRING_CONST",
 /*  22 */ "expression_to_print ::= expression",
 /*  23 */ "cond ::=",
 /*  24 */ "else_start ::= TKN_ELSE",
 /*  25 */ "cond ::= else_start body",
 /*  26 */ "expression ::= exp",
 /*  27 */ "expression ::= exp comp exp",
 /*  28 */ "exp_begin ::= termino sign",
 /*  29 */ "termino ::= factor",
 /*  30 */ "termino_begin ::= factor oper",
 /*  31 */ "factor_prm ::= TKN_ID",
 /*  32 */ "cte_int ::= TKN_INT_CONST",
 /*  33 */ "cte_float ::= TKN_FLOAT_CONST",
 /*  34 */ "comp ::= TKN_LT",
 /*  35 */ "comp ::= TKN_GT",
 /*  36 */ "comp ::= TKN_NE",
 /*  37 */ "program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions program_main body program_end",
 /*  38 */ "vars ::= TKN_VAR vars_prm",
 /*  39 */ "vars ::=",
 /*  40 */ "functions ::= function functions",
 /*  41 */ "functions ::=",
 /*  42 */ "function ::= TKN_VOID function_start TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end",
 /*  43 */ "params ::= param_def TKN_COMMA params",
 /*  44 */ "body ::= TKN_LBRACE statements TKN_RBRACE",
 /*  45 */ "statements ::= statement statements",
 /*  46 */ "statements ::=",
 /*  47 */ "statement ::= assign",
 /*  48 */ "statement ::= condition",
 /*  49 */ "statement ::= cycle",
 /*  50 */ "statement ::= f_call",
 /*  51 */ "statement ::= print",
 /*  52 */ "assign ::= assign_start expression TKN_SEMI_COLON",
 /*  53 */ "condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON",
 /*  54 */ "cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end",
 /*  55 */ "call ::=",
 /*  56 */ "call ::= call_expression call_prm",
 /*  57 */ "call_prm ::= TKN_COMMA call",
 /*  58 */ "call_prm ::=",
 /*  59 */ "print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON",
 /*  60 */ "print_prm ::= string_to_print print_prm_prm",
 /*  61 */ "print_prm ::= expression_to_print print_prm_prm",
 /*  62 */ "print_prm_prm ::= TKN_COMMA print_prm",
 /*  63 */ "print_prm_prm ::=",
 /*  64 */ "exp ::= termino",
 /*  65 */ "exp ::= exp_begin exp",
 /*  66 */ "termino ::= termino_begin termino",
 /*  67 */ "factor ::= TKN_LPAREN expression TKN_RPAREN",
 /*  68 */ "factor ::= sign factor_prm",
 /*  69 */ "factor ::= factor_prm",
 /*  70 */ "factor_prm ::= cte_int",
 /*  71 */ "factor_prm ::= cte_float",
 /*  72 */ "oper ::= TKN_MULT",
 /*  73 */ "oper ::= TKN_DIV",
 /*  74 */ "sign ::= TKN_PLUS",
 /*  75 */ "sign ::= TKN_MINUS",
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
      /* TERMINAL Destructor */
    case 1: /* TKN_PROGRAM */
    case 2: /* TKN_SEMI_COLON */
    case 3: /* TKN_ID */
    case 4: /* TKN_MAIN */
    case 5: /* TKN_END */
    case 6: /* TKN_VAR */
    case 7: /* TKN_COLON */
    case 8: /* TKN_COMMA */
    case 9: /* TKN_INT */
    case 10: /* TKN_FLOAT */
    case 11: /* TKN_VOID */
    case 12: /* TKN_LPAREN */
    case 13: /* TKN_RPAREN */
    case 14: /* TKN_LBRACKET */
    case 15: /* TKN_RBRACKET */
    case 16: /* TKN_LBRACE */
    case 17: /* TKN_RBRACE */
    case 18: /* TKN_ASSIGN */
    case 19: /* TKN_IF */
    case 20: /* TKN_WHILE */
    case 21: /* TKN_PRINT */
    case 22: /* TKN_STRING_CONST */
    case 23: /* TKN_ELSE */
    case 24: /* TKN_INT_CONST */
    case 25: /* TKN_FLOAT_CONST */
    case 26: /* TKN_MULT */
    case 27: /* TKN_DIV */
    case 28: /* TKN_PLUS */
    case 29: /* TKN_MINUS */
    case 30: /* TKN_LT */
    case 31: /* TKN_GT */
    case 32: /* TKN_NE */
    case 33: /* TKN_DO */
{
#line 10 "syntax/example.y"

  if ((yypminor->yy0)) {
    //printf("Destructor called on string: [%s]\n", (yypminor->yy0));
    free((yypminor->yy0));
  }

#line 724 "syntax/example.c"
}
      break;
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
  {   42,   -1 }, /* (0) program_id ::= TKN_ID */
  {   45,   -1 }, /* (1) program_main ::= TKN_MAIN */
  {   47,   -1 }, /* (2) program_end ::= TKN_END */
  {   48,   -4 }, /* (3) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON */
  {   48,   -5 }, /* (4) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm */
  {   34,   -1 }, /* (5) vars_prm_prm ::= TKN_ID */
  {   34,   -3 }, /* (6) vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm */
  {   39,   -1 }, /* (7) type ::= TKN_INT */
  {   39,   -1 }, /* (8) type ::= TKN_FLOAT */
  {   50,   -1 }, /* (9) function_start ::= TKN_ID */
  {   35,   -3 }, /* (10) params ::= TKN_ID TKN_COLON type */
  {   36,   -3 }, /* (11) param_def ::= TKN_ID TKN_COLON type */
  {   51,   -1 }, /* (12) function_end ::= TKN_SEMI_COLON */
  {   59,   -2 }, /* (13) assign_start ::= TKN_ID TKN_ASSIGN */
  {   61,   -1 }, /* (14) condition_start ::= TKN_RPAREN */
  {   63,   -2 }, /* (15) cycle_condition_start ::= TKN_WHILE TKN_LPAREN */
  {   64,   -1 }, /* (16) cycle_expression ::= expression */
  {   65,   -1 }, /* (17) cycle_end ::= TKN_SEMI_COLON */
  {   57,   -4 }, /* (18) f_call ::= f_call_start call TKN_RPAREN TKN_SEMI_COLON */
  {   40,   -2 }, /* (19) f_call_start ::= TKN_ID TKN_LPAREN */
  {   67,   -1 }, /* (20) call_expression ::= expression */
  {   70,   -1 }, /* (21) string_to_print ::= TKN_STRING_CONST */
  {   71,   -1 }, /* (22) expression_to_print ::= expression */
  {   62,    0 }, /* (23) cond ::= */
  {   73,   -1 }, /* (24) else_start ::= TKN_ELSE */
  {   62,   -2 }, /* (25) cond ::= else_start body */
  {   60,   -1 }, /* (26) expression ::= exp */
  {   60,   -3 }, /* (27) expression ::= exp comp exp */
  {   77,   -2 }, /* (28) exp_begin ::= termino sign */
  {   76,   -1 }, /* (29) termino ::= factor */
  {   80,   -2 }, /* (30) termino_begin ::= factor oper */
  {   82,   -1 }, /* (31) factor_prm ::= TKN_ID */
  {   38,   -1 }, /* (32) cte_int ::= TKN_INT_CONST */
  {   37,   -1 }, /* (33) cte_float ::= TKN_FLOAT_CONST */
  {   75,   -1 }, /* (34) comp ::= TKN_LT */
  {   75,   -1 }, /* (35) comp ::= TKN_GT */
  {   75,   -1 }, /* (36) comp ::= TKN_NE */
  {   41,   -8 }, /* (37) program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions program_main body program_end */
  {   43,   -2 }, /* (38) vars ::= TKN_VAR vars_prm */
  {   43,    0 }, /* (39) vars ::= */
  {   44,   -2 }, /* (40) functions ::= function functions */
  {   44,    0 }, /* (41) functions ::= */
  {   49,  -10 }, /* (42) function ::= TKN_VOID function_start TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end */
  {   35,   -3 }, /* (43) params ::= param_def TKN_COMMA params */
  {   46,   -3 }, /* (44) body ::= TKN_LBRACE statements TKN_RBRACE */
  {   52,   -2 }, /* (45) statements ::= statement statements */
  {   52,    0 }, /* (46) statements ::= */
  {   53,   -1 }, /* (47) statement ::= assign */
  {   53,   -1 }, /* (48) statement ::= condition */
  {   53,   -1 }, /* (49) statement ::= cycle */
  {   53,   -1 }, /* (50) statement ::= f_call */
  {   53,   -1 }, /* (51) statement ::= print */
  {   54,   -3 }, /* (52) assign ::= assign_start expression TKN_SEMI_COLON */
  {   55,   -7 }, /* (53) condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON */
  {   56,   -5 }, /* (54) cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end */
  {   66,    0 }, /* (55) call ::= */
  {   66,   -2 }, /* (56) call ::= call_expression call_prm */
  {   68,   -2 }, /* (57) call_prm ::= TKN_COMMA call */
  {   68,    0 }, /* (58) call_prm ::= */
  {   58,   -5 }, /* (59) print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON */
  {   69,   -2 }, /* (60) print_prm ::= string_to_print print_prm_prm */
  {   69,   -2 }, /* (61) print_prm ::= expression_to_print print_prm_prm */
  {   72,   -2 }, /* (62) print_prm_prm ::= TKN_COMMA print_prm */
  {   72,    0 }, /* (63) print_prm_prm ::= */
  {   74,   -1 }, /* (64) exp ::= termino */
  {   74,   -2 }, /* (65) exp ::= exp_begin exp */
  {   76,   -2 }, /* (66) termino ::= termino_begin termino */
  {   79,   -3 }, /* (67) factor ::= TKN_LPAREN expression TKN_RPAREN */
  {   79,   -2 }, /* (68) factor ::= sign factor_prm */
  {   79,   -1 }, /* (69) factor ::= factor_prm */
  {   82,   -1 }, /* (70) factor_prm ::= cte_int */
  {   82,   -1 }, /* (71) factor_prm ::= cte_float */
  {   81,   -1 }, /* (72) oper ::= TKN_MULT */
  {   81,   -1 }, /* (73) oper ::= TKN_DIV */
  {   78,   -1 }, /* (74) sign ::= TKN_PLUS */
  {   78,   -1 }, /* (75) sign ::= TKN_MINUS */
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
#line 32 "syntax/example.y"
{
  programStart(yymsp[0].minor.yy0, ctx);
  //yymsp[0].minor.yy0 = strdup(yymsp[0].minor.yy0);
}
#line 1188 "syntax/example.c"
        break;
      case 1: /* program_main ::= TKN_MAIN */
{  yy_destructor(yypParser,4,&yymsp[0].minor);
#line 36 "syntax/example.y"
{
  foundMain(ctx);
}
#line 1196 "syntax/example.c"
}
        break;
      case 2: /* program_end ::= TKN_END */
{  yy_destructor(yypParser,5,&yymsp[0].minor);
#line 39 "syntax/example.y"
{
  //printf("End of program\n");
  //ctx->programFunction = NULL;
  //ctx->currentFunction = NULL;
}
#line 1207 "syntax/example.c"
}
        break;
      case 3: /* vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON */
#line 47 "syntax/example.y"
{
  //printf("TYPE: %s\n", yymsp[-1].minor.yy0);
  handleVariableList(yymsp[-3].minor.yy86, ctx, yymsp[-1].minor.yy0);
}
#line 1216 "syntax/example.c"
  yy_destructor(yypParser,7,&yymsp[-2].minor);
  yy_destructor(yypParser,2,&yymsp[0].minor);
        break;
      case 4: /* vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm */
#line 51 "syntax/example.y"
{
  //printf("TYPE: %s\n", yymsp[-2].minor.yy0);
  handleVariableList(yymsp[-4].minor.yy86, ctx, yymsp[-2].minor.yy0);
}
#line 1226 "syntax/example.c"
  yy_destructor(yypParser,7,&yymsp[-3].minor);
  yy_destructor(yypParser,2,&yymsp[-1].minor);
        break;
      case 5: /* vars_prm_prm ::= TKN_ID */
#line 55 "syntax/example.y"
{
  yylhsminor.yy86 = malloc(sizeof(StringList));
  yylhsminor.yy86 ->id = strdup(yymsp[0].minor.yy0);
  yylhsminor.yy86 ->next = NULL;
}
#line 1237 "syntax/example.c"
  yymsp[0].minor.yy86 = yylhsminor.yy86;
        break;
      case 6: /* vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm */
#line 60 "syntax/example.y"
{
  yylhsminor.yy86 = malloc(sizeof(StringList));
  yylhsminor.yy86 ->id = strdup(yymsp[-2].minor.yy0);
  yylhsminor.yy86 ->next = yymsp[0].minor.yy86;
}
#line 1247 "syntax/example.c"
  yy_destructor(yypParser,8,&yymsp[-1].minor);
  yymsp[-2].minor.yy86 = yylhsminor.yy86;
        break;
      case 7: /* type ::= TKN_INT */
      case 8: /* type ::= TKN_FLOAT */ yytestcase(yyruleno==8);
#line 66 "syntax/example.y"
{
  yylhsminor.yy0 = yymsp[0].minor.yy0;
}
#line 1257 "syntax/example.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 9: /* function_start ::= TKN_ID */
#line 78 "syntax/example.y"
{
  functionStart(yymsp[0].minor.yy0, ctx);

  //printf("Current Function: %s\n",yymsp[0].minor.yy0);
}
#line 1267 "syntax/example.c"
        break;
      case 10: /* params ::= TKN_ID TKN_COLON type */
      case 11: /* param_def ::= TKN_ID TKN_COLON type */ yytestcase(yyruleno==11);
#line 84 "syntax/example.y"
{
  yylhsminor.yy86 = malloc(sizeof(StringList*));
  yylhsminor.yy86->id = strdup(yymsp[-2].minor.yy0);
  yylhsminor.yy86->next = NULL;
  handleVariableList(yylhsminor.yy86, ctx, yymsp[0].minor.yy0);
  addToFunctionSignature(yymsp[0].minor.yy0, ctx);
}
#line 1279 "syntax/example.c"
  yy_destructor(yypParser,7,&yymsp[-1].minor);
  yymsp[-2].minor.yy86 = yylhsminor.yy86;
        break;
      case 12: /* function_end ::= TKN_SEMI_COLON */
{  yy_destructor(yypParser,2,&yymsp[0].minor);
#line 102 "syntax/example.y"
{
  functionEnd(ctx);
  //printf("Exiting function\n");
}
#line 1290 "syntax/example.c"
}
        break;
      case 13: /* assign_start ::= TKN_ID TKN_ASSIGN */
#line 118 "syntax/example.y"
{
  handleOperator(yymsp[-1].minor.yy0, ctx);
  handleOperation(yymsp[0].minor.yy0, ctx);
}
#line 1299 "syntax/example.c"
        break;
      case 14: /* condition_start ::= TKN_RPAREN */
{  yy_destructor(yypParser,13,&yymsp[0].minor);
#line 124 "syntax/example.y"
{
  handleConditionStart(ctx);
}
#line 1307 "syntax/example.c"
}
        break;
      case 15: /* cycle_condition_start ::= TKN_WHILE TKN_LPAREN */
{  yy_destructor(yypParser,20,&yymsp[-1].minor);
#line 129 "syntax/example.y"
{
  handleCycleConditionStart(ctx);
}
#line 1316 "syntax/example.c"
  yy_destructor(yypParser,12,&yymsp[0].minor);
}
        break;
      case 16: /* cycle_expression ::= expression */
#line 132 "syntax/example.y"
{
  handleCycleStart(ctx);
}
#line 1325 "syntax/example.c"
        break;
      case 17: /* cycle_end ::= TKN_SEMI_COLON */
{  yy_destructor(yypParser,2,&yymsp[0].minor);
#line 135 "syntax/example.y"
{
  handleCycleEnd(ctx);
}
#line 1333 "syntax/example.c"
}
        break;
      case 18: /* f_call ::= f_call_start call TKN_RPAREN TKN_SEMI_COLON */
#line 140 "syntax/example.y"
{
  functionCallEnd(yymsp[-3].minor.yy0, ctx);
}
#line 1341 "syntax/example.c"
  yy_destructor(yypParser,13,&yymsp[-1].minor);
  yy_destructor(yypParser,2,&yymsp[0].minor);
        break;
      case 19: /* f_call_start ::= TKN_ID TKN_LPAREN */
#line 144 "syntax/example.y"
{
  functionCallStart(yymsp[-1].minor.yy0, ctx);
  yylhsminor.yy0 = yymsp[-1].minor.yy0;
}
#line 1351 "syntax/example.c"
  yy_destructor(yypParser,12,&yymsp[0].minor);
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 20: /* call_expression ::= expression */
#line 150 "syntax/example.y"
{
  functionParameterAdded(yymsp[0].minor.yy0, ctx);
}
#line 1360 "syntax/example.c"
        break;
      case 21: /* string_to_print ::= TKN_STRING_CONST */
#line 159 "syntax/example.y"
{
  handlePrintString(yymsp[0].minor.yy0, ctx);
}
#line 1367 "syntax/example.c"
        break;
      case 22: /* expression_to_print ::= expression */
#line 163 "syntax/example.y"
{
  //printf("Got exp: %s\n", yymsp[0].minor.yy0);
  handlePrintExpression(yymsp[0].minor.yy0, ctx);
}
#line 1375 "syntax/example.c"
        break;
      case 23: /* cond ::= */
#line 173 "syntax/example.y"
{
  handleConditionEnd(ctx);
}
#line 1382 "syntax/example.c"
        break;
      case 24: /* else_start ::= TKN_ELSE */
{  yy_destructor(yypParser,23,&yymsp[0].minor);
#line 178 "syntax/example.y"
{
  handleConditionElseStart(ctx);
}
#line 1390 "syntax/example.c"
}
        break;
      case 25: /* cond ::= else_start body */
#line 181 "syntax/example.y"
{
  handleConditionElseEnd(ctx);
}
#line 1398 "syntax/example.c"
        break;
      case 26: /* expression ::= exp */
      case 27: /* expression ::= exp comp exp */ yytestcase(yyruleno==27);
#line 186 "syntax/example.y"
{
  quadsSolve(ctx);
}
#line 1406 "syntax/example.c"
        break;
      case 28: /* exp_begin ::= termino sign */
      case 34: /* comp ::= TKN_LT */ yytestcase(yyruleno==34);
      case 35: /* comp ::= TKN_GT */ yytestcase(yyruleno==35);
      case 36: /* comp ::= TKN_NE */ yytestcase(yyruleno==36);
#line 194 "syntax/example.y"
{
  handleOperation(yymsp[0].minor.yy0, ctx);
}
#line 1416 "syntax/example.c"
        break;
      case 29: /* termino ::= factor */
#line 200 "syntax/example.y"
{
  //handleOperator(yymsp[0].minor.yy0, ctx);
}
#line 1423 "syntax/example.c"
        break;
      case 30: /* termino_begin ::= factor oper */
#line 204 "syntax/example.y"
{
  //handleOperator(yymsp[-1].minor.yy0, ctx);
  handleOperation(yymsp[0].minor.yy0, ctx);
}
#line 1431 "syntax/example.c"
        break;
      case 31: /* factor_prm ::= TKN_ID */
#line 215 "syntax/example.y"
{
  handleOperator(yymsp[0].minor.yy0, ctx);
}
#line 1438 "syntax/example.c"
        break;
      case 32: /* cte_int ::= TKN_INT_CONST */
#line 224 "syntax/example.y"
{
  addConstantInt(yymsp[0].minor.yy0, ctx);
}
#line 1445 "syntax/example.c"
        break;
      case 33: /* cte_float ::= TKN_FLOAT_CONST */
#line 228 "syntax/example.y"
{
  addConstantFloat(yymsp[0].minor.yy0, ctx);
}
#line 1452 "syntax/example.c"
        break;
      case 37: /* program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions program_main body program_end */
{  yy_destructor(yypParser,1,&yymsp[-7].minor);
#line 30 "syntax/example.y"
{
}
#line 1459 "syntax/example.c"
  yy_destructor(yypParser,2,&yymsp[-5].minor);
}
        break;
      case 38: /* vars ::= TKN_VAR vars_prm */
{  yy_destructor(yypParser,6,&yymsp[-1].minor);
#line 45 "syntax/example.y"
{
}
#line 1468 "syntax/example.c"
}
        break;
      case 42: /* function ::= TKN_VOID function_start TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end */
{  yy_destructor(yypParser,11,&yymsp[-9].minor);
#line 76 "syntax/example.y"
{
}
#line 1476 "syntax/example.c"
  yy_destructor(yypParser,12,&yymsp[-7].minor);
  yy_destructor(yypParser,13,&yymsp[-5].minor);
  yy_destructor(yypParser,14,&yymsp[-4].minor);
  yy_destructor(yypParser,15,&yymsp[-1].minor);
}
        break;
      case 43: /* params ::= param_def TKN_COMMA params */
#line 92 "syntax/example.y"
{
}
#line 1487 "syntax/example.c"
  yy_destructor(yypParser,8,&yymsp[-1].minor);
        break;
      case 44: /* body ::= TKN_LBRACE statements TKN_RBRACE */
{  yy_destructor(yypParser,16,&yymsp[-2].minor);
#line 107 "syntax/example.y"
{
}
#line 1495 "syntax/example.c"
  yy_destructor(yypParser,17,&yymsp[0].minor);
}
        break;
      case 52: /* assign ::= assign_start expression TKN_SEMI_COLON */
#line 122 "syntax/example.y"
{
}
#line 1503 "syntax/example.c"
  yy_destructor(yypParser,2,&yymsp[0].minor);
        break;
      case 53: /* condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON */
{  yy_destructor(yypParser,19,&yymsp[-6].minor);
#line 127 "syntax/example.y"
{
}
#line 1511 "syntax/example.c"
  yy_destructor(yypParser,12,&yymsp[-5].minor);
  yy_destructor(yypParser,2,&yymsp[0].minor);
}
        break;
      case 54: /* cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end */
#line 138 "syntax/example.y"
{
}
#line 1520 "syntax/example.c"
  yy_destructor(yypParser,13,&yymsp[-2].minor);
        break;
      case 57: /* call_prm ::= TKN_COMMA call */
      case 62: /* print_prm_prm ::= TKN_COMMA print_prm */ yytestcase(yyruleno==62);
{  yy_destructor(yypParser,8,&yymsp[-1].minor);
#line 154 "syntax/example.y"
{
}
#line 1529 "syntax/example.c"
}
        break;
      case 59: /* print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON */
{  yy_destructor(yypParser,21,&yymsp[-4].minor);
#line 157 "syntax/example.y"
{
}
#line 1537 "syntax/example.c"
  yy_destructor(yypParser,12,&yymsp[-3].minor);
  yy_destructor(yypParser,13,&yymsp[-1].minor);
  yy_destructor(yypParser,2,&yymsp[0].minor);
}
        break;
      case 67: /* factor ::= TKN_LPAREN expression TKN_RPAREN */
{  yy_destructor(yypParser,12,&yymsp[-2].minor);
#line 210 "syntax/example.y"
{
}
#line 1548 "syntax/example.c"
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 72: /* oper ::= TKN_MULT */
{  yy_destructor(yypParser,26,&yymsp[0].minor);
#line 232 "syntax/example.y"
{
}
#line 1557 "syntax/example.c"
}
        break;
      case 73: /* oper ::= TKN_DIV */
{  yy_destructor(yypParser,27,&yymsp[0].minor);
#line 233 "syntax/example.y"
{
}
#line 1565 "syntax/example.c"
}
        break;
      case 74: /* sign ::= TKN_PLUS */
{  yy_destructor(yypParser,28,&yymsp[0].minor);
#line 235 "syntax/example.y"
{
}
#line 1573 "syntax/example.c"
}
        break;
      case 75: /* sign ::= TKN_MINUS */
{  yy_destructor(yypParser,29,&yymsp[0].minor);
#line 236 "syntax/example.y"
{
}
#line 1581 "syntax/example.c"
}
        break;
      default:
      /* (39) vars ::= */ yytestcase(yyruleno==39);
      /* (40) functions ::= function functions */ yytestcase(yyruleno==40);
      /* (41) functions ::= */ yytestcase(yyruleno==41);
      /* (45) statements ::= statement statements */ yytestcase(yyruleno==45);
      /* (46) statements ::= */ yytestcase(yyruleno==46);
      /* (47) statement ::= assign (OPTIMIZED OUT) */ assert(yyruleno!=47);
      /* (48) statement ::= condition (OPTIMIZED OUT) */ assert(yyruleno!=48);
      /* (49) statement ::= cycle (OPTIMIZED OUT) */ assert(yyruleno!=49);
      /* (50) statement ::= f_call (OPTIMIZED OUT) */ assert(yyruleno!=50);
      /* (51) statement ::= print (OPTIMIZED OUT) */ assert(yyruleno!=51);
      /* (55) call ::= */ yytestcase(yyruleno==55);
      /* (56) call ::= call_expression call_prm */ yytestcase(yyruleno==56);
      /* (58) call_prm ::= */ yytestcase(yyruleno==58);
      /* (60) print_prm ::= string_to_print print_prm_prm */ yytestcase(yyruleno==60);
      /* (61) print_prm ::= expression_to_print print_prm_prm */ yytestcase(yyruleno==61);
      /* (63) print_prm_prm ::= */ yytestcase(yyruleno==63);
      /* (64) exp ::= termino */ yytestcase(yyruleno==64);
      /* (65) exp ::= exp_begin exp */ yytestcase(yyruleno==65);
      /* (66) termino ::= termino_begin termino */ yytestcase(yyruleno==66);
      /* (68) factor ::= sign factor_prm */ yytestcase(yyruleno==68);
      /* (69) factor ::= factor_prm (OPTIMIZED OUT) */ assert(yyruleno!=69);
      /* (70) factor_prm ::= cte_int (OPTIMIZED OUT) */ assert(yyruleno!=70);
      /* (71) factor_prm ::= cte_float (OPTIMIZED OUT) */ assert(yyruleno!=71);
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
#line 25 "syntax/example.y"

  printf("Syntax error! At: %s" , yyminor);
  exit(1);
#line 1671 "syntax/example.c"
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
