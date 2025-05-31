%{
#include "../syntax/example.h"
#include "../utils/stringUtils.h"

extern void *parser;  // referencia al parser Lemon
extern void Parse(void *parser, int token, char* value, void *scanner);  // función generada por Lemon

char string_buffer[1024];
int string_index = 0;
%}

%option noyywrap
%option reentrant
%option extra-type="char **"

%x STRING

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

\"                        { string_index = 0; BEGIN(STRING); }

<STRING>[^\"\n]+         {
    int len = yyleng;
    if (string_index + len < sizeof(string_buffer)) {
        strncpy(&string_buffer[string_index], yytext, len);
        string_index += len;
    }
}

<STRING>\\.               {
    if (string_index + 2 < sizeof(string_buffer)) {
        string_buffer[string_index++] = yytext[0];
        string_buffer[string_index++] = yytext[1];
    }
}

<STRING>\"                {
    string_buffer[string_index] = '\0';
    *yyextra = strdup(string_buffer);  // raw string
    BEGIN(INITIAL);
    return TKN_STRING_CONST;
}

[a-zA-Z][0-9a-zA-Z_]*     { return TKN_ID; }
[0-9]+                    { return TKN_INT_CONST; }
[0-9]+"."[0-9]*           { return TKN_FLOAT_CONST; }
[ \t\n\r]+                    { /* ignore whitespace */ }

%%