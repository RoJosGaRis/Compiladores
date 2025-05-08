// parser_context.h
#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

typedef struct{
    struct FunctionEntry * functionTable;
    struct FunctionEntry * currentFunction;
    struct FunctionEntry * programFunction;
} ParserContext;

#endif

