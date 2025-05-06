%{
#include "../syntax/example.h"
%}

%option noyywrap
%option reentrant

%x DOUBLE_QUOTED

%%

"program"                 { return TKN_PROGRAM; }
"main"                    { return TKN_MAIN; }
"end"                     { return TKN_END; }
"var"                     { return TKN_VAR; }
"int"                     { return TKN_INT; }
"float"                   { return TKN_FLOAT; }
"print"                   { return TKN_PRINT; }
"void"                    { return TKN_VOID; }
"if"                      { return TKN_IF; }
"else"                    { return TKN_ELSE; }
"while"                   { return TKN_WHILE; }
"do"                      { return TKN_DO; }
";"                       { return TKN_SEMI_COLON; }
","                       { return TKN_COMMA; }
":"                       { return TKN_COLON; }
"{"                       { return TKN_LBRACE; }
"}"                       { return TKN_RBRACE; }
"("                       { return TKN_LPAREN; }
")"                       { return TKN_RPAREN; }
"["                       { return TKN_LBRACKET; }
"]"                       { return TKN_RBRACKET; }
"+"                       { return TKN_PLUS; }
"-"                       { return TKN_MINUS; }
"*"                       { return TKN_MULT; }
"/"                       { return TKN_DIV; }
"!="                      { return TKN_NE; }
"="                       { return TKN_ASSIGN; }
"<"                       { return TKN_LT; }
">"                       { return TKN_GT; }
["]                       {BEGIN(DOUBLE_QUOTED);}
<DOUBLE_QUOTED>[^"]+      {}
<DOUBLE_QUOTED>["]        {BEGIN(INITIAL); return TKN_STRING_CONST;}
<DOUBLE_QUOTED><<EOF>>    {return -1;}
[a-zA-Z][0-9a-zA-Z_]*     { return TKN_ID; }
[0-9]+                    { return TKN_INT_CONST; }
[0-9]+"."[0-9]*           { return TKN_FLOAT_CONST; }
[ \t\n\r]+                    { /* ignore whitespace */ }

%%