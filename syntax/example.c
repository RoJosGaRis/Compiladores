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
#line 6 "syntax/example.y"

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
#define YYNOCODE 76
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE char*
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  StringList* yy50;
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
#define YYNRULE              69
#define YYNTOKEN             34
#define YY_MAX_SHIFT         68
#define YY_MIN_SHIFTREDUCE   124
#define YY_MAX_SHIFTREDUCE   192
#define YY_ERROR_ACTION      193
#define YY_ACCEPT_ACTION     194
#define YY_NO_ACTION         195
#define YY_MIN_REDUCE        196
#define YY_MAX_REDUCE        264
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
#define YY_ACTTAB_COUNT (227)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    32,   43,  249,   50,   14,   23,   13,  245,   23,   23,
 /*    10 */    20,  199,   24,   11,   14,   23,   13,   32,   23,   23,
 /*    20 */   181,  251,  251,  227,   55,  194,   48,   20,   17,   24,
 /*    30 */    11,   14,   23,   13,  209,   23,   23,   67,   57,   31,
 /*    40 */   187,  188,   17,  140,   20,   39,   24,   11,   14,   23,
 /*    50 */    13,  257,   23,   23,   12,   20,   39,   24,   11,   14,
 /*    60 */    23,   13,   64,   23,   23,   44,   20,   45,   24,   11,
 /*    70 */    14,   23,   13,   38,   23,   23,   50,  147,  148,  149,
 /*    80 */    60,   58,   56,   20,  223,   24,   11,   14,   23,   13,
 /*    90 */    61,   23,   23,    5,  153,  154,  189,  190,   49,  134,
 /*   100 */    20,   62,   24,   11,   14,   23,   13,   46,   23,   23,
 /*   110 */   218,   20,  216,   24,   11,   14,   23,   13,  204,   23,
 /*   120 */    23,  231,    9,    9,    9,    9,    9,    9,    7,  133,
 /*   130 */   191,  192,    3,   65,    9,    9,    9,    9,    9,    9,
 /*   140 */     7,  229,   10,  201,    3,   47,   16,   52,   27,  248,
 /*   150 */   130,   24,   11,   14,   23,   13,   53,   23,   23,  181,
 /*   160 */     1,  244,  243,  138,  240,   34,  215,    8,   24,   11,
 /*   170 */    14,   23,   13,  181,   23,   23,  213,   33,   59,  187,
 /*   180 */   188,    8,   18,  191,  192,   36,  135,   37,    4,  256,
 /*   190 */   125,   66,   19,  187,  188,  124,   40,  191,  192,   68,
 /*   200 */    25,   21,   26,   28,   15,   22,  170,   29,  167,   51,
 /*   210 */    54,   30,    2,  166,   35,  136,  169,    6,  222,   41,
 /*   220 */   178,   63,   42,  195,  195,  195,  158,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    55,    1,   67,   34,   69,   70,   71,   62,   73,   74,
 /*    10 */    65,   42,   67,   68,   69,   70,   71,   55,   73,   74,
 /*    20 */     4,   73,   74,   39,   62,   36,    4,   65,   44,   67,
 /*    30 */    68,   69,   70,   71,   55,   73,   74,   39,   59,   11,
 /*    40 */    24,   25,   44,   23,   65,   55,   67,   68,   69,   70,
 /*    50 */    71,   61,   73,   74,   66,   65,   55,   67,   68,   69,
 /*    60 */    70,   71,   61,   73,   74,    4,   65,   43,   67,   68,
 /*    70 */    69,   70,   71,   55,   73,   74,   34,   30,   31,   32,
 /*    80 */    19,   20,   21,   65,   42,   67,   68,   69,   70,   71,
 /*    90 */    55,   73,   74,   12,    9,   10,   26,   27,   43,   18,
 /*   100 */    65,   55,   67,   68,   69,   70,   71,    4,   73,   74,
 /*   110 */    72,   65,   69,   67,   68,   69,   70,   71,   35,   73,
 /*   120 */    74,   47,   48,   49,   50,   51,   52,   53,   54,    2,
 /*   130 */    28,   29,   58,   47,   48,   49,   50,   51,   52,   53,
 /*   140 */    54,   46,   16,   34,   58,   40,    6,   35,   38,   65,
 /*   150 */     4,   67,   68,   69,   70,   71,   45,   73,   74,    4,
 /*   160 */     8,   63,   63,    2,   60,   40,   65,   12,   67,   68,
 /*   170 */    69,   70,   71,    4,   73,   74,   40,   22,   57,   24,
 /*   180 */    25,   12,   40,   28,   29,   64,   13,   56,    8,   75,
 /*   190 */     5,   41,   38,   24,   25,    4,   40,   28,   29,   37,
 /*   200 */     8,    7,   15,    8,    2,    7,    2,   14,    2,   13,
 /*   210 */    13,   12,   12,    2,   13,   12,    2,   12,    0,    3,
 /*   220 */    13,   13,    2,   76,   76,   76,   17,   76,   76,   76,
 /*   230 */    76,   76,   76,   76,   76,   76,   76,   76,   76,   76,
 /*   240 */    76,   76,   76,   76,   76,   76,   76,   76,   76,   76,
 /*   250 */    76,   76,   76,   76,   76,   76,   76,   76,   76,   76,
 /*   260 */    76,
};
#define YY_SHIFT_COUNT    (68)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (220)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */     0,  155,  155,  169,  169,  169,  169,  169,  169,   61,
 /*    10 */    61,  169,  169,  169,   16,   22,   22,   28,   20,   28,
 /*    20 */    47,   85,   85,   70,  102,  103,  127,  126,   22,  140,
 /*    30 */   103,  146,  152,  152,  161,  126,  126,  126,  173,  180,
 /*    40 */   185,  126,  140,  191,   81,  192,  194,  187,  195,  202,
 /*    50 */   198,  193,  196,  199,  204,  197,  200,  201,  203,  206,
 /*    60 */   205,  211,  207,  214,  208,  209,  218,  216,  220,
};
#define YY_REDUCE_COUNT (43)
#define YY_REDUCE_MIN   (-65)
#define YY_REDUCE_MAX   (162)
static const short yy_reduce_ofst[] = {
 /*     0 */   -11,  -55,  -38,  -21,  -10,    1,   18,   35,   46,   74,
 /*    10 */    86,   84,  101,  -65,  -52,  -31,   42,  -16,  121,   -2,
 /*    20 */   -12,   24,   55,   38,   43,   83,   95,  105,  109,  110,
 /*    30 */   112,  111,   98,   99,  104,  125,  136,  142,  131,  114,
 /*    40 */   150,  156,  154,  162,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   193,  193,  193,  193,  255,  255,  193,  193,  193,  232,
 /*    10 */   232,  193,  193,  193,  193,  198,  193,  228,  211,  228,
 /*    20 */   214,  193,  193,  217,  247,  193,  193,  193,  193,  224,
 /*    30 */   193,  193,  246,  246,  193,  193,  193,  193,  193,  258,
 /*    40 */   193,  193,  224,  193,  193,  203,  193,  193,  200,  193,
 /*    50 */   193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
 /*    60 */   193,  193,  193,  193,  193,  193,  193,  193,  193,
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
  /*   36 */ "program",
  /*   37 */ "program_id",
  /*   38 */ "vars",
  /*   39 */ "functions",
  /*   40 */ "body",
  /*   41 */ "program_end",
  /*   42 */ "vars_prm",
  /*   43 */ "type",
  /*   44 */ "function",
  /*   45 */ "function_id",
  /*   46 */ "function_end",
  /*   47 */ "statements",
  /*   48 */ "statement",
  /*   49 */ "assign",
  /*   50 */ "condition",
  /*   51 */ "cycle",
  /*   52 */ "f_call",
  /*   53 */ "print",
  /*   54 */ "assign_start",
  /*   55 */ "expression",
  /*   56 */ "condition_start",
  /*   57 */ "cond",
  /*   58 */ "cycle_condition_start",
  /*   59 */ "cycle_expression",
  /*   60 */ "cycle_end",
  /*   61 */ "call",
  /*   62 */ "print_prm",
  /*   63 */ "print_prm_prm",
  /*   64 */ "else_start",
  /*   65 */ "exp",
  /*   66 */ "comp",
  /*   67 */ "termino",
  /*   68 */ "exp_begin",
  /*   69 */ "sign",
  /*   70 */ "factor",
  /*   71 */ "termino_begin",
  /*   72 */ "oper",
  /*   73 */ "factor_prm",
  /*   74 */ "cte",
  /*   75 */ "call_prm",
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
 /*  15 */ "cond ::=",
 /*  16 */ "else_start ::= TKN_ELSE",
 /*  17 */ "cond ::= else_start body",
 /*  18 */ "expression ::= exp",
 /*  19 */ "expression ::= exp comp exp",
 /*  20 */ "exp_begin ::= termino sign",
 /*  21 */ "termino ::= factor",
 /*  22 */ "termino_begin ::= factor oper",
 /*  23 */ "comp ::= TKN_LT",
 /*  24 */ "comp ::= TKN_GT",
 /*  25 */ "comp ::= TKN_NE",
 /*  26 */ "program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions TKN_MAIN body program_end",
 /*  27 */ "vars ::= TKN_VAR vars_prm",
 /*  28 */ "vars ::=",
 /*  29 */ "type ::= TKN_INT",
 /*  30 */ "type ::= TKN_FLOAT",
 /*  31 */ "functions ::= function functions",
 /*  32 */ "functions ::=",
 /*  33 */ "function ::= TKN_VOID function_id TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end",
 /*  34 */ "body ::= TKN_LBRACE statements TKN_RBRACE",
 /*  35 */ "statements ::= statement statements",
 /*  36 */ "statements ::=",
 /*  37 */ "statement ::= assign",
 /*  38 */ "statement ::= condition",
 /*  39 */ "statement ::= cycle",
 /*  40 */ "statement ::= f_call",
 /*  41 */ "statement ::= print",
 /*  42 */ "assign ::= assign_start expression TKN_SEMI_COLON",
 /*  43 */ "condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON",
 /*  44 */ "cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end",
 /*  45 */ "f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON",
 /*  46 */ "print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON",
 /*  47 */ "print_prm ::= TKN_STRING_CONST print_prm_prm",
 /*  48 */ "print_prm ::= expression print_prm_prm",
 /*  49 */ "print_prm_prm ::= TKN_COMMA print_prm",
 /*  50 */ "print_prm_prm ::=",
 /*  51 */ "exp ::= termino",
 /*  52 */ "exp ::= exp_begin exp",
 /*  53 */ "termino ::= termino_begin termino",
 /*  54 */ "factor ::= TKN_LPAREN expression TKN_RPAREN",
 /*  55 */ "factor ::= sign factor_prm",
 /*  56 */ "factor ::= factor_prm",
 /*  57 */ "factor_prm ::= TKN_ID",
 /*  58 */ "factor_prm ::= cte",
 /*  59 */ "call ::=",
 /*  60 */ "call ::= expression call_prm",
 /*  61 */ "call_prm ::= TKN_COMMA call",
 /*  62 */ "call_prm ::=",
 /*  63 */ "cte ::= TKN_INT_CONST",
 /*  64 */ "cte ::= TKN_FLOAT_CONST",
 /*  65 */ "oper ::= TKN_MULT",
 /*  66 */ "oper ::= TKN_DIV",
 /*  67 */ "sign ::= TKN_PLUS",
 /*  68 */ "sign ::= TKN_MINUS",
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
  {   37,   -1 }, /* (0) program_id ::= TKN_ID */
  {   41,   -1 }, /* (1) program_end ::= TKN_END */
  {   42,   -4 }, /* (2) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON */
  {   42,   -5 }, /* (3) vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm */
  {   34,   -1 }, /* (4) vars_prm_prm ::= TKN_ID */
  {   34,   -3 }, /* (5) vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm */
  {   45,   -1 }, /* (6) function_id ::= TKN_ID */
  {   35,   -3 }, /* (7) params ::= TKN_ID TKN_COLON type */
  {   35,   -5 }, /* (8) params ::= TKN_ID TKN_COLON type TKN_COMMA params */
  {   46,   -1 }, /* (9) function_end ::= TKN_SEMI_COLON */
  {   54,   -2 }, /* (10) assign_start ::= TKN_ID TKN_ASSIGN */
  {   56,   -1 }, /* (11) condition_start ::= TKN_RPAREN */
  {   58,   -2 }, /* (12) cycle_condition_start ::= TKN_WHILE TKN_LPAREN */
  {   59,   -1 }, /* (13) cycle_expression ::= expression */
  {   60,   -1 }, /* (14) cycle_end ::= TKN_SEMI_COLON */
  {   57,    0 }, /* (15) cond ::= */
  {   64,   -1 }, /* (16) else_start ::= TKN_ELSE */
  {   57,   -2 }, /* (17) cond ::= else_start body */
  {   55,   -1 }, /* (18) expression ::= exp */
  {   55,   -3 }, /* (19) expression ::= exp comp exp */
  {   68,   -2 }, /* (20) exp_begin ::= termino sign */
  {   67,   -1 }, /* (21) termino ::= factor */
  {   71,   -2 }, /* (22) termino_begin ::= factor oper */
  {   66,   -1 }, /* (23) comp ::= TKN_LT */
  {   66,   -1 }, /* (24) comp ::= TKN_GT */
  {   66,   -1 }, /* (25) comp ::= TKN_NE */
  {   36,   -8 }, /* (26) program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions TKN_MAIN body program_end */
  {   38,   -2 }, /* (27) vars ::= TKN_VAR vars_prm */
  {   38,    0 }, /* (28) vars ::= */
  {   43,   -1 }, /* (29) type ::= TKN_INT */
  {   43,   -1 }, /* (30) type ::= TKN_FLOAT */
  {   39,   -2 }, /* (31) functions ::= function functions */
  {   39,    0 }, /* (32) functions ::= */
  {   44,  -10 }, /* (33) function ::= TKN_VOID function_id TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end */
  {   40,   -3 }, /* (34) body ::= TKN_LBRACE statements TKN_RBRACE */
  {   47,   -2 }, /* (35) statements ::= statement statements */
  {   47,    0 }, /* (36) statements ::= */
  {   48,   -1 }, /* (37) statement ::= assign */
  {   48,   -1 }, /* (38) statement ::= condition */
  {   48,   -1 }, /* (39) statement ::= cycle */
  {   48,   -1 }, /* (40) statement ::= f_call */
  {   48,   -1 }, /* (41) statement ::= print */
  {   49,   -3 }, /* (42) assign ::= assign_start expression TKN_SEMI_COLON */
  {   50,   -7 }, /* (43) condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON */
  {   51,   -5 }, /* (44) cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end */
  {   52,   -5 }, /* (45) f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON */
  {   53,   -5 }, /* (46) print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON */
  {   62,   -2 }, /* (47) print_prm ::= TKN_STRING_CONST print_prm_prm */
  {   62,   -2 }, /* (48) print_prm ::= expression print_prm_prm */
  {   63,   -2 }, /* (49) print_prm_prm ::= TKN_COMMA print_prm */
  {   63,    0 }, /* (50) print_prm_prm ::= */
  {   65,   -1 }, /* (51) exp ::= termino */
  {   65,   -2 }, /* (52) exp ::= exp_begin exp */
  {   67,   -2 }, /* (53) termino ::= termino_begin termino */
  {   70,   -3 }, /* (54) factor ::= TKN_LPAREN expression TKN_RPAREN */
  {   70,   -2 }, /* (55) factor ::= sign factor_prm */
  {   70,   -1 }, /* (56) factor ::= factor_prm */
  {   73,   -1 }, /* (57) factor_prm ::= TKN_ID */
  {   73,   -1 }, /* (58) factor_prm ::= cte */
  {   61,    0 }, /* (59) call ::= */
  {   61,   -2 }, /* (60) call ::= expression call_prm */
  {   75,   -2 }, /* (61) call_prm ::= TKN_COMMA call */
  {   75,    0 }, /* (62) call_prm ::= */
  {   74,   -1 }, /* (63) cte ::= TKN_INT_CONST */
  {   74,   -1 }, /* (64) cte ::= TKN_FLOAT_CONST */
  {   72,   -1 }, /* (65) oper ::= TKN_MULT */
  {   72,   -1 }, /* (66) oper ::= TKN_DIV */
  {   69,   -1 }, /* (67) sign ::= TKN_PLUS */
  {   69,   -1 }, /* (68) sign ::= TKN_MINUS */
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
#line 21 "syntax/example.y"
{
  addFunction(&ctx->functionTable, yymsp[0].minor.yy0, "void");
  ctx->programFunction = findFunction(&ctx->functionTable, yymsp[0].minor.yy0);
  ctx->currentFunction = NULL;
  initContext(ctx);
  yymsp[0].minor.yy0 = strdup(yymsp[0].minor.yy0);
  //printf("Program name: %s\n", yymsp[0].minor.yy0);
}
#line 1120 "syntax/example.c"
        break;
      case 1: /* program_end ::= TKN_END */
#line 30 "syntax/example.y"
{
  //printf("End of program\n");
  ctx->programFunction = NULL;
  ctx->currentFunction = NULL;
}
#line 1129 "syntax/example.c"
        break;
      case 2: /* vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON */
#line 38 "syntax/example.y"
{
  handleVariableList(yymsp[-3].minor.yy50, ctx, yymsp[-1].minor.yy0);
}
#line 1136 "syntax/example.c"
        break;
      case 3: /* vars_prm ::= vars_prm_prm TKN_COLON type TKN_SEMI_COLON vars_prm */
#line 41 "syntax/example.y"
{
  handleVariableList(yymsp[-4].minor.yy50, ctx, yymsp[-2].minor.yy0);
}
#line 1143 "syntax/example.c"
        break;
      case 4: /* vars_prm_prm ::= TKN_ID */
#line 44 "syntax/example.y"
{
  yylhsminor.yy50 = malloc(sizeof(StringList));
  yylhsminor.yy50 ->id = strdup(yymsp[0].minor.yy0);
  yylhsminor.yy50 ->next = NULL;
}
#line 1152 "syntax/example.c"
  yymsp[0].minor.yy50 = yylhsminor.yy50;
        break;
      case 5: /* vars_prm_prm ::= TKN_ID TKN_COMMA vars_prm_prm */
#line 49 "syntax/example.y"
{
  yylhsminor.yy50 = malloc(sizeof(StringList));
  yylhsminor.yy50 ->id = strdup(yymsp[-2].minor.yy0);
  yylhsminor.yy50 ->next = yymsp[0].minor.yy50;
}
#line 1162 "syntax/example.c"
  yymsp[-2].minor.yy50 = yylhsminor.yy50;
        break;
      case 6: /* function_id ::= TKN_ID */
#line 63 "syntax/example.y"
{
  addFunction(&ctx->functionTable, yymsp[0].minor.yy0, "void");
  ctx->currentFunction = findFunction(&ctx->functionTable, yymsp[0].minor.yy0);
  //printf("Current Function: %s\n",yymsp[0].minor.yy0);
}
#line 1172 "syntax/example.c"
        break;
      case 7: /* params ::= TKN_ID TKN_COLON type */
#line 69 "syntax/example.y"
{
  yylhsminor.yy50 = malloc(sizeof(StringList*));
  yylhsminor.yy50->id = strdup(yymsp[-2].minor.yy0);
  yylhsminor.yy50->next = NULL;
  handleVariableList(yylhsminor.yy50, ctx, yymsp[0].minor.yy0);
}
#line 1182 "syntax/example.c"
  yymsp[-2].minor.yy50 = yylhsminor.yy50;
        break;
      case 8: /* params ::= TKN_ID TKN_COLON type TKN_COMMA params */
#line 75 "syntax/example.y"
{
  yylhsminor.yy50 = malloc(sizeof(StringList*));
  yylhsminor.yy50->id = strdup(yymsp[-4].minor.yy0);
  yylhsminor.yy50->next = NULL;
  handleVariableList(yylhsminor.yy50, ctx, yymsp[-2].minor.yy0);
}
#line 1193 "syntax/example.c"
  yymsp[-4].minor.yy50 = yylhsminor.yy50;
        break;
      case 9: /* function_end ::= TKN_SEMI_COLON */
#line 82 "syntax/example.y"
{
  ctx->currentFunction = NULL;
  //printf("Exiting function\n");
}
#line 1202 "syntax/example.c"
        break;
      case 10: /* assign_start ::= TKN_ID TKN_ASSIGN */
      case 22: /* termino_begin ::= factor oper */ yytestcase(yyruleno==22);
#line 98 "syntax/example.y"
{
  handleOperator(yymsp[-1].minor.yy0, ctx);
  handleOperation(yymsp[0].minor.yy0, ctx);
}
#line 1211 "syntax/example.c"
        break;
      case 11: /* condition_start ::= TKN_RPAREN */
#line 104 "syntax/example.y"
{
  handleConditionStart(ctx);
}
#line 1218 "syntax/example.c"
        break;
      case 12: /* cycle_condition_start ::= TKN_WHILE TKN_LPAREN */
#line 109 "syntax/example.y"
{
  handleCycleConditionStart(ctx);
}
#line 1225 "syntax/example.c"
        break;
      case 13: /* cycle_expression ::= expression */
#line 112 "syntax/example.y"
{
  handleCycleStart(ctx);
}
#line 1232 "syntax/example.c"
        break;
      case 14: /* cycle_end ::= TKN_SEMI_COLON */
#line 115 "syntax/example.y"
{
  handleCycleEnd(ctx);
}
#line 1239 "syntax/example.c"
        break;
      case 15: /* cond ::= */
#line 129 "syntax/example.y"
{
  handleConditionEnd(ctx);
}
#line 1246 "syntax/example.c"
        break;
      case 16: /* else_start ::= TKN_ELSE */
#line 134 "syntax/example.y"
{
  handleConditionElseStart(ctx);
}
#line 1253 "syntax/example.c"
        break;
      case 17: /* cond ::= else_start body */
#line 137 "syntax/example.y"
{
  handleConditionElseEnd(ctx);
}
#line 1260 "syntax/example.c"
        break;
      case 18: /* expression ::= exp */
      case 19: /* expression ::= exp comp exp */ yytestcase(yyruleno==19);
#line 142 "syntax/example.y"
{
  quadsSolve(ctx);
}
#line 1268 "syntax/example.c"
        break;
      case 20: /* exp_begin ::= termino sign */
      case 23: /* comp ::= TKN_LT */ yytestcase(yyruleno==23);
      case 24: /* comp ::= TKN_GT */ yytestcase(yyruleno==24);
      case 25: /* comp ::= TKN_NE */ yytestcase(yyruleno==25);
#line 150 "syntax/example.y"
{
  handleOperation(yymsp[0].minor.yy0, ctx);
}
#line 1278 "syntax/example.c"
        break;
      case 21: /* termino ::= factor */
#line 156 "syntax/example.y"
{
  handleOperator(yymsp[0].minor.yy0, ctx);
}
#line 1285 "syntax/example.c"
        break;
      default:
      /* (26) program ::= TKN_PROGRAM program_id TKN_SEMI_COLON vars functions TKN_MAIN body program_end */ yytestcase(yyruleno==26);
      /* (27) vars ::= TKN_VAR vars_prm */ yytestcase(yyruleno==27);
      /* (28) vars ::= */ yytestcase(yyruleno==28);
      /* (29) type ::= TKN_INT */ yytestcase(yyruleno==29);
      /* (30) type ::= TKN_FLOAT */ yytestcase(yyruleno==30);
      /* (31) functions ::= function functions */ yytestcase(yyruleno==31);
      /* (32) functions ::= */ yytestcase(yyruleno==32);
      /* (33) function ::= TKN_VOID function_id TKN_LPAREN params TKN_RPAREN TKN_LBRACKET vars body TKN_RBRACKET function_end */ yytestcase(yyruleno==33);
      /* (34) body ::= TKN_LBRACE statements TKN_RBRACE */ yytestcase(yyruleno==34);
      /* (35) statements ::= statement statements */ yytestcase(yyruleno==35);
      /* (36) statements ::= */ yytestcase(yyruleno==36);
      /* (37) statement ::= assign (OPTIMIZED OUT) */ assert(yyruleno!=37);
      /* (38) statement ::= condition (OPTIMIZED OUT) */ assert(yyruleno!=38);
      /* (39) statement ::= cycle (OPTIMIZED OUT) */ assert(yyruleno!=39);
      /* (40) statement ::= f_call (OPTIMIZED OUT) */ assert(yyruleno!=40);
      /* (41) statement ::= print (OPTIMIZED OUT) */ assert(yyruleno!=41);
      /* (42) assign ::= assign_start expression TKN_SEMI_COLON */ yytestcase(yyruleno==42);
      /* (43) condition ::= TKN_IF TKN_LPAREN expression condition_start body cond TKN_SEMI_COLON */ yytestcase(yyruleno==43);
      /* (44) cycle ::= cycle_condition_start cycle_expression TKN_RPAREN body cycle_end */ yytestcase(yyruleno==44);
      /* (45) f_call ::= TKN_ID TKN_LPAREN call TKN_RPAREN TKN_SEMI_COLON */ yytestcase(yyruleno==45);
      /* (46) print ::= TKN_PRINT TKN_LPAREN print_prm TKN_RPAREN TKN_SEMI_COLON */ yytestcase(yyruleno==46);
      /* (47) print_prm ::= TKN_STRING_CONST print_prm_prm */ yytestcase(yyruleno==47);
      /* (48) print_prm ::= expression print_prm_prm */ yytestcase(yyruleno==48);
      /* (49) print_prm_prm ::= TKN_COMMA print_prm */ yytestcase(yyruleno==49);
      /* (50) print_prm_prm ::= */ yytestcase(yyruleno==50);
      /* (51) exp ::= termino */ yytestcase(yyruleno==51);
      /* (52) exp ::= exp_begin exp */ yytestcase(yyruleno==52);
      /* (53) termino ::= termino_begin termino */ yytestcase(yyruleno==53);
      /* (54) factor ::= TKN_LPAREN expression TKN_RPAREN */ yytestcase(yyruleno==54);
      /* (55) factor ::= sign factor_prm */ yytestcase(yyruleno==55);
      /* (56) factor ::= factor_prm (OPTIMIZED OUT) */ assert(yyruleno!=56);
      /* (57) factor_prm ::= TKN_ID */ yytestcase(yyruleno==57);
      /* (58) factor_prm ::= cte (OPTIMIZED OUT) */ assert(yyruleno!=58);
      /* (59) call ::= */ yytestcase(yyruleno==59);
      /* (60) call ::= expression call_prm */ yytestcase(yyruleno==60);
      /* (61) call_prm ::= TKN_COMMA call */ yytestcase(yyruleno==61);
      /* (62) call_prm ::= */ yytestcase(yyruleno==62);
      /* (63) cte ::= TKN_INT_CONST */ yytestcase(yyruleno==63);
      /* (64) cte ::= TKN_FLOAT_CONST */ yytestcase(yyruleno==64);
      /* (65) oper ::= TKN_MULT */ yytestcase(yyruleno==65);
      /* (66) oper ::= TKN_DIV */ yytestcase(yyruleno==66);
      /* (67) sign ::= TKN_PLUS */ yytestcase(yyruleno==67);
      /* (68) sign ::= TKN_MINUS */ yytestcase(yyruleno==68);
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
#line 15 "syntax/example.y"

  printf("Syntax error! At: %s" , yyminor);
#line 1393 "syntax/example.c"
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
