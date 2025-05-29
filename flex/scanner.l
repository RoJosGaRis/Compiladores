%{
#include "../syntax/example.h"

extern void *parser;  // referencia al parser Lemon
extern void Parse(void *parser, int token, char* value, void *scanner);  // función generada por Lemon

char string_buffer[1024];
int string_index = 0;
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
\"                        { string_index = 0; BEGIN(DOUBLE_QUOTED); }

<DOUBLE_QUOTED>[^\"\n]+   {
                            int len = yyleng;
                            if (string_index + len >= sizeof(string_buffer)) {
                                fprintf(stderr, "String literal too long\n");
                                exit(1);
                            }
                            strncpy(&string_buffer[string_index], yytext, len);
                            string_index += len;
                          }

<DOUBLE_QUOTED>\"         {
                            string_buffer[string_index] = '\0';
                            BEGIN(INITIAL);
                            Parse(parser, TKN_STRING_CONST, string_buffer, NULL); // <- pasa string a parser
                            return TKN_STRING_CONST;
                          }

<DOUBLE_QUOTED><<EOF>>    {
                            fprintf(stderr, "Unterminated string constant\n");
                            exit(1);
                          }

[a-zA-Z][0-9a-zA-Z_]*     { return TKN_ID; }
[0-9]+                    { return TKN_INT_CONST; }
[0-9]+"."[0-9]*           { return TKN_FLOAT_CONST; }
[ \t\n\r]+                    { /* ignore whitespace */ }

%%