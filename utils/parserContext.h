// parser_context.h
#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include <stdio.h>
#include <stdlib.h>
// #include "types.h"
#include "quads/quads.h"
#include "hashDriver.h"
// #include "../consts.h"
// #include "structs.h"

void initMemoryMap(ParserContext * ctx);
void initContext(ParserContext * ctx);
void printContextVariables(ParserContext * ctx);
int handleQuad(int id1, int id2, OPERATORS op, ParserContext * ctx);
void handleOperation(char * op, ParserContext * ctx);
void handleOperator(char * id1, ParserContext * ctx);
int quadsSolve(ParserContext * ctx);
void handleConditionStart(ParserContext * ctx);
void handleConditionEnd(ParserContext * ctx);

#endif

